import json
from datasets import load_dataset

print("Downloading 10,000 Wikipedia articles...")
dataset = load_dataset("wikimedia/wikipedia", "20231101.simple", split="train", streaming=True)

documents = []
for i, item in enumerate(dataset):
    if i >= 10000:
        break
    documents.append({
        "doc_id": i,
        "url": item["url"],
        "title": item["title"],
        "content": item["text"]
    })
    if (i + 1) % 1000 == 0:
        print(f"Processed {i + 1}/10,000 articles...")

with open("scraped_pages.json", "w", encoding="utf-8") as f:
    json.dump(documents, f, ensure_ascii=False)

print("✅ Saved 10,000 real Wikipedia articles to scraped_pages.json!")