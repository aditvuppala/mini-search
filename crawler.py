import json
import re
import time
import urllib.robotparser
from urllib.parse import urljoin, urlparse
import requests
from bs4 import BeautifulSoup


class MiniCrawler:
    def __init__(self, seed_url, max_pages=10):
        self.seed_url = seed_url
        self.max_pages = max_pages
        self.visited = set()
        self.documents = []
        self.allowed_domain = urlparse(seed_url).netloc
        self.user_agent = "MiniSearchCrawler/1.0"

        # 1. Initialize and load robots.txt parser
        self.rp = urllib.robotparser.RobotFileParser()
        self._load_robots_txt()

    def _load_robots_txt(self):
        """Fetches and parses the domain's robots.txt file."""
        parsed = urlparse(self.seed_url)
        robots_url = f"{parsed.scheme}://{parsed.netloc}/robots.txt"
        try:
            self.rp.set_url(robots_url)
            self.rp.read()
            print(f"[Robots.txt] Successfully loaded rules from: {robots_url}")
        except Exception as e:
            print(f"[Robots.txt] Note: Could not read robots.txt ({e}). Defaulting to allow all.")

    def can_fetch(self, url):
        """Checks if our User-Agent is permitted to crawl this specific URL."""
        return self.rp.can_fetch(self.user_agent, url)

    def clean_text(self, soup):
        """Removes markup boilerplate (scripts, navbars, styles) and extracts clean plain text."""
        for element in soup(["script", "style", "nav", "footer", "header"]):
            element.extract()

        text = soup.get_text(separator=' ')
        return re.sub(r'\s+', ' ', text).strip()

    def crawl(self):
        """Executes a Breadth-First Search (BFS) crawl up to max_pages."""
        queue = [self.seed_url]
        doc_id = 0

        print(f"\n🚀 Starting crawl at: {self.seed_url} (Limit: {self.max_pages} pages)")

        while queue and len(self.visited) < self.max_pages:
            url = queue.pop(0)

            # Skip if already visited
            if url in self.visited:
                continue

            # Respect robots.txt exclusions
            if not self.can_fetch(url):
                print(f"🚫 [Robots.txt Block] Skipping: {url}")
                continue

            try:
                print(f"[{len(self.visited) + 1}/{self.max_pages}] Crawling: {url}")
                response = requests.get(
                    url,
                    timeout=5,
                    headers={"User-Agent": self.user_agent}
                )

                # Skip non-HTML responses (e.g. PDFs, binary downloads, images)
                content_type = response.headers.get("Content-Type", "")
                if "text/html" not in content_type:
                    continue

                self.visited.add(url)
                soup = BeautifulSoup(response.text, "html.parser")

                # Extract page title & cleaned text body
                title = soup.title.string.strip() if soup.title and soup.title.string else url
                clean_content = self.clean_text(soup)

                # Store document for inverted index indexing
                self.documents.append({
                    "doc_id": doc_id,
                    "url": url,
                    "title": title,
                    "content": clean_content
                })
                doc_id += 1

                # Discover and queue internal hyperlinks
                for link in soup.find_all("a", href=True):
                    full_url = urljoin(url, link["href"]).split('#')[0]

                    # Stay within target domain and enforce HTTP/HTTPS
                    if urlparse(full_url).netloc == self.allowed_domain and full_url.startswith("http"):
                        if full_url not in self.visited and full_url not in queue:
                            queue.append(full_url)

                # Politeness delay to prevent rate-limiting/server load
                time.sleep(0.5)

            except Exception as e:
                print(f"⚠️ Failed to crawl {url}: {e}")

        # Save indexed documents to JSON
        with open("scraped_pages.json", "w", encoding="utf-8") as f:
            json.dump(self.documents, f, indent=2, ensure_ascii=False)

        print(f"\n✅ Crawl complete! Saved {len(self.documents)} pages to 'scraped_pages.json'")


if __name__ == "__main__":
    crawler = MiniCrawler(seed_url="https://books.toscrape.com/", max_pages=5)
    crawler.crawl()