import requests
from bs4 import BeautifulSoup
import json
import re
from urllib.parse import urljoin, urlparse
import time

class MiniCrawler:
    def __init__(self, seed_url, max_pages=10):
        self.seed_url = seed_url
        self.max_pages = max_pages
        self.visited = set()
        self.documents = []
        self.allowed_domain = urlparse(seed_url).netloc

    def clean_text(self, soup):
        #remove js/css elements
        for element in soup(["script", "style", "nav", "footer", "header"]):
            element.extract()

        # Extract text and collapse whitespace
        text = soup.get_text(separator=' ')
        return re.sub(r'\s+', ' ', text).strip()

    def crawl(self):
        queue = [self.seed_url]
        doc_id = 0

        print(f"Starting crawl at: {self.seed_url}")

        while queue and len(self.visited) < self.max_pages:
            url = queue.pop(0)

            if url in self.visited:
                continue

            try:
                print(f"[{len(self.visited) + 1}/{self.max_pages}] Crawling: {url}")
                response = requests.get(url, timeout=5, headers={"User-Agent": "MiniSearchCrawler/1.0"})
                
                # Only process HTML
                if "text/html" not in response.headers.get("Content-Type", ""):
                    continue

                self.visited.add(url)
                soup = BeautifulSoup(response.text, "html.parser")

                # Extract title & content
                title = soup.title.string.strip() if soup.title and soup.title.string else url
                clean_content = self.clean_text(soup)

                # store scraped page object
                self.documents.append({
                    "doc_id": doc_id,
                    "url": url,
                    "title": title,
                    "content": clean_content
                })
                doc_id += 1

                # Extract internal links for queue
                for link in soup.find_all("a", href=True):
                    full_url = urljoin(url, link["href"])
                    # Remove fragments (#section)
                    full_url = full_url.split('#')[0]

                    # Stay within same domain and avoid non-http links
                    if urlparse(full_url).netloc == self.allowed_domain and full_url.startswith("http"):
                        if full_url not in self.visited and full_url not in queue:
                            queue.append(full_url)

                time.sleep(0.5)

            except Exception as e:
                print(f"Failed to crawl {url}: {e}")

        # Save result to JSON file for C++ to read
        with open("scraped_pages.json", "w", encoding="utf-8") as f:
            json.dump(self.documents, f, indent=2, ensure_ascii=False)

        print(f"\n✅ Crawl complete! Saved {len(self.documents)} pages to 'scraped_pages.json'")

if __name__ == "__main__":
    # Test crawl on a clean, simple target site
    crawler = MiniCrawler(seed_url="https://books.toscrape.com/", max_pages=5)
    crawler.crawl()
        