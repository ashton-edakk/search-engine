// NAME: Ashton Edakkunnathu
// UIN: 665685590
// CLASS: CS 251 - Data Structures
// PROFESSOR: Ethan Ordentlich
// Project: 2 - Search Engine
// DATE: 2/13/2024
// IED: Visual Studio Code
// FILE: search.h

#pragma once

#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/**
 * Cleans a specific token by:
 * - Removing leading and trailing punctuation
 * - Converting to lowercase
 * If the token has no letters, returns the empty string.
 */
string cleanToken(string s) {
  // TODO_STUDENT
  if (s.empty()) { // if the word is emppty return  ""
    return "";
  }

  size_t start = 0;
  while (start < s.length() &&
         ispunct(s[start])) { // finds the start of the word
    start++;
  }

  size_t end = s.length();
  while (end > start && ispunct(s[end - 1])) { // finds the end of the word
    end--;
  }

  if (start == end) {
    return "";
  }

  for (size_t i = start; i < end; i++) { // convert all the letters to lowercase
    s[i] = tolower(s[i]);
  }

  // creates the cleaned up tag
  string cleanedtag = s.substr(start, end - start);

  bool hasletter = false;
  for (char c : cleanedtag) {
    if (isalpha(c)) {
      hasletter = true;
    }
  }
  if (!hasletter) { ////if there are no letters, return ""
    return "";
  }

  return s.substr(start, end - start); // return the clean tag
}

/**
 * Returns a set of the cleaned tokens in the given text.
 */
set<string> gatherTokens(string text) {
  // TODO_STUDENT

  set<string> result;
  set<string>::iterator it;

  istringstream iss(text);
  string token;

  while (iss >> token) { // breaks all of the string text into words by
                         // whitespace, putting them into a set
    result.insert(cleanToken(token));
  }

  // I am going to be so for real, I don't know why this makes the function work
  // but it does ¯\(°_o)/¯ You and I both know "sadfvcsccdscds" doesn't exist in
  // results, so it defaults to result.end(), yet when I use result.end() in lui
  // of en lieu result.find("sadfvcsccdscds"), the code doesn't work.
  it = result.find("");
  if (it != result.find("sadfvcsccdscds")) {
    result.erase(it); // erases any possible empty indexes
  }

  return result;
}

/**
 * Builds the inverted index from the given data file. The existing contents
 * of `index` are left in place.
 */
int buildIndex(string filename, map<string, set<string>> &index) {
  // TODO_STUDENT
  ifstream file(filename);
  if (!file.is_open()) {
    return -1; // indicate failure
  }

  string line;
  string url;      // link
  string content;  // url content
  int counter = 0; // counter

  while (getline(file, line)) {
    // Read URL
    url = line;
    counter++;

    // Read content
    if (getline(file, line)) {
      content = line;

      set<string> tokens =
          gatherTokens(content); // creates all the tokens in the url

      for (const string &tag :
           tokens) { // adds the current url into the set of each tag in the map
        index[tag].insert(url);
      }
    } else { // if there is no content for the url
      cout << "Incomplete data for URL: " << url
           << ". Give me the content of it. You silly goose." << endl;
    }
  }

  file.close();
  return counter;
}

/**
 * Runs a search query on the provided index, and returns a set of results.
 *
 * Search terms are processed using set operations (union, intersection,
 * difference).
 */
set<string> findQueryMatches(map<string, set<string>> &index, string sentence) {
  // TODO_STUDENT
  set<string> results;
  vector<string> tags;
  string word;

  // Split the sentence into tags
  for (int i = 0; i < sentence.size(); i++) {
    if (sentence[i] != ' ') {
      word += sentence[i];
    } else {
      if (!word.empty()) {
        tags.push_back(word);
        word.clear();
      }
    }
  }

  if (!word.empty()) { // catches the last word
    tags.push_back(word);
  }

  bool first = true; // if it is the first go around

  for (int i = 0; i < tags.size(); i++) {
    string tag = tags[i]; // sets the tag we are going to look at

    char sign = tag[0]; // checks the modifier for the word

    tag = cleanToken(tag);

    if (first && sign != '-') { // if it is the first time we look at a word
                                // with the '-' mofidier,
      results = index[tag];
      first = false;
    } else if (sign == '+') { // performs the set intersection
      set<string> intersection;
      for (const string &url :
           results) { // goes through all the urls in results and if it is  in
                      // the tag set we add it
        if (index[tag].find(url) != index[tag].end()) {
          intersection.insert(url);
        }
      }
      results = intersection;
    } else if (sign == '-') { // performs the set difference       also side
                              // note the '-' looks like a cute little face.
      for (auto it = results.begin();
           it != results.end();) { // iterates through results and if the value
                                   // is not in the tag index it gets remvoed
        if (index[tag].find(*it) != index[tag].end()) {
          it = results.erase(it);
        } else {
          ++it;
        }
      }
    } else { // performs the set union
      for (const string &url :
           index[tag]) { // simply adds the urls to the results
        results.insert(url);
      }
    }
  }

  return results;
}

/**
 * Runs the main command loop for the search program
 */
void searchEngine(string filename) {
  // TODO_STUDENT
  map<string, set<string>> index;
  string command; // tags

  int numPages = buildIndex(filename, index);
  if (numPages <= 0) { // for in case the file is invalid
    cout << "Invalid filename." << endl;
    cout << "Stand by while building index..." << endl;
    cout << "Indexed 0 pages containing 0 unique terms" << endl;
  } else { // prints out all the pages and unique term numbers
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << numPages << " pages containing " << index.size()
         << " unique terms" << endl;
  }

  do {
    cout << endl << "Enter query sentence (press enter to quit): ";
    getline(cin, command); // gets the command

    if (command.empty()) { // exits the code
      cout << "Thank you for searching!" << endl;
      return;
    }
    set<string> results =
        findQueryMatches(index, command); // creates all the results
    cout << "Found " << results.size() << " matching pages" << endl;
    for (const string &url : results) { // prints out all the url results
      cout << url << endl;
    }
  } while (true);
}
