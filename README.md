# Rudimentary Search Engine (Text-Based)

## Overview
This project is a **rudimentary search engine** that accepts user-provided keywords and returns a list of URLs that contain those keywords. Both the URLs and their associated content are stored as **plain text (`.txt`) files**, making the system simple, lightweight, and easy to understand.

The goal of this project is to demonstrate basic information retrieval concepts such as keyword matching, file parsing, and result aggregation without using external libraries or databases.

---

## Features
- Keyword-based search
- Searches both URL names and URL content
- Uses plain text files for storage
- Case-insensitive keyword matching
- Simple command-line interface
- Modular and extensible design

---

## How It Works
1. URLs are represented as `.txt` files.
3. The user enters one or more keywords.
4. The search engine scans all text files:
   - Checks if the keywords appear in the URL name or file content
5. Matching URLs are returned as search results.
