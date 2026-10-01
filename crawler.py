import json
import re
import time
import urllib.robotparser
from urllib.parse import urljoin, urlparse
import requests
from bs4 import BeautifulSoup
import concurrent.futures



class MiniCrawler:
    def __init__(self, seed_url, max_pages=300, max_workers = 8):
        self.seed_url = seed_url
        self.max_pages = max_pages
        self.max_workers = max_workers
        self.visited = set()
        self.documents = []
        self.allowed_domain = urlparse(seed_url).netloc
        self.user_agent = "MiniSearchCrawler/1.0"
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
        # Strip scripts, styles, navigation bars, footers, and sidebar categories
        for element in soup(["script", "style", "nav", "footer", "header", "aside"]):
            element.extract()
            
        for sidebar in soup.find_all("div", class_="side_categories"):
            sidebar.extract()

        text = soup.get_text(separator=' ')
        return re.sub(r'\s+', ' ', text).strip()

    def fetch_page(self, url):
        if not self.can_fetch(url):
            return None, []
        try:
            resp = requests.get(url, timeout=5, headers={"User-Agent": self.user_agent})
            if "text/html" not in resp.headers.get("Content-Type", ""):
                return None, []

            soup = BeautifulSoup(resp.text, "html.parser")
            title = soup.title.string.strip() if soup.title and soup.title.string else url
            clean_content = self.clean_text(soup)

            links = []
            for link in soup.find_all("a", href=True):
                full_url = urljoin(url, link["href"]).split('#')[0]
                if urlparse(full_url).netloc == self.allowed_domain and full_url.startswith("http"):
                    links.append(full_url)

            return {"url": url, "title": title, "content": clean_content}, links
        except Exception:
            return None, []

    def crawl(self):
        queue = [self.seed_url]
        self.visited.add(self.seed_url)
        doc_id = 0

        print(f"\n🚀 Starting crawl at: {self.seed_url} (Limit: {self.max_pages} pages)")

        with concurrent.futures.ThreadPoolExecutor(max_workers=self.max_workers) as executor:
            while queue and len(self.documents) < self.max_pages:
                # Take up to max_workers URLs from the queue
                batch = queue[:self.max_workers]
                queue = queue[self.max_workers:]

                # Fetch the batch concurrently
                futures = {executor.submit(self.fetch_page, url): url for url in batch}

                for future in concurrent.futures.as_completed(futures):
                    doc, links = future.result()
                    if doc and len(self.documents) < self.max_pages:
                        doc["doc_id"] = doc_id
                        self.documents.append(doc)
                        doc_id += 1
                        print(f"[{len(self.documents)}/{self.max_pages}] Crawling: {doc['url']}")

                    for link in links:
                        if link not in self.visited:
                            self.visited.add(link)
                            queue.append(link)

        # Save result to JSON file for C++ to read
        with open("scraped_pages.json", "w", encoding="utf-8") as f:
            json.dump(self.documents, f, indent=2, ensure_ascii=False)

        print(f"\n✅ Crawl complete! Saved {len(self.documents)} pages to 'scraped_pages.json'")


if __name__ == "__main__":
    crawler = MiniCrawler(seed_url="https://books.toscrape.com/", max_pages=300, max_workers=10)
    crawler.crawl()