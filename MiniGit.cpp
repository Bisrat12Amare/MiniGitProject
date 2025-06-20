#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include "MiniGit.hpp"
#include <set>
#include <map>

MiniGit::MiniGit() {}

void MiniGit::createDirectory(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
        std::cout << "Created: " << path << "\n";
    }
}

void MiniGit::init() {
    std::cout << "Initializing MiniGit Repository...\n";

    createDirectory(repoPath);
    createDirectory(objectsPath);
    createDirectory(repoPath + "/refs");
    createDirectory(commitsPath);

    std::ofstream headFile(repoPath + "/HEAD");
    if (headFile) {
        headFile << "ref: refs/master\n";
        headFile.close();
        std::cout << "Initialized HEAD to master branch.\n";
    } else {
        std::cerr << "Failed to write HEAD file.\n";
    }
}

std::string MiniGit::readFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("File not found: " + filename);
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::string MiniGit::computeHash(const std::string& content) {
    size_t sum = 0;
    for (char c : content) sum += static_cast<unsigned char>(c);
    return std::to_string(sum); 
}

void MiniGit::writeBlob(const std::string& hash, const std::string& content) {
    std::string path = objectsPath + "/" + hash;
    std::ofstream out(path);
    if (out) {
        out << content;
        std::cout << "Saved blob: " << path << "\n";
    } else {
        std::cerr << "Failed to write blob file.\n";
    }
}

void MiniGit::add(const std::string& filename) {
    try {
        std::string content = readFile(filename);
        std::string hash = computeHash(content);

        if (stagedFiles.find(filename) == stagedFiles.end()) {
            stagedFiles.insert(filename);
            writeBlob(hash, content);
            std::cout << "Staged: " << filename << "\n";
        } else {
            std::cout << filename << " is already staged.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Add failed: " << e.what() << "\n";
    }
}
void MiniGit::commit(const std::string& message) {
    if (stagedFiles.empty()) {
        std::cout << "No changes to commit.\n";
        return;
    }

    Commit commit;
    commit.message = message;
    commit.timestamp = std::time(nullptr);
    commit.parentHash = headHash;

    for (const auto& file : stagedFiles) {
        commit.stagedFiles.push_back(file);
    }

    commit.hash = generateCommitHash(commit);
    saveCommit(commit);
    headHash = commit.hash;

    stagedFiles.clear();

    std::cout << "Committed with hash: " << commit.hash << "\n";
}

void MiniGit::saveCommit(const Commit& commit) {
    std::string path = commitsPath + "/" + commit.hash;
    std::ofstream out(path);
    if (out) {
        out << "Message: " << commit.message << "\n";
        out << "Timestamp: " << commit.timestamp << "\n";
        out << "Parent: " << commit.parentHash << "\n";
        out << "Files:\n";
        for (const auto& f : commit.stagedFiles)
            out << f << "\n";
    } else {
        std::cerr << "Failed to save commit.\n";
    }

    std::ofstream headFile(repoPath + "/HEAD");
    if (headFile) {
        headFile << "ref: " << commit.hash << "\n";
    }
}

std::string MiniGit::generateCommitHash(const Commit& commit) {
    size_t sum = 0;
    sum += std::hash<std::string>{}(commit.message);
    sum += commit.timestamp;
    for (const auto& file : commit.stagedFiles)
        sum += std::hash<std::string>{}(file);
    return std::to_string(sum);
}
void MiniGit::log() {
    std::string currentHash = headHash;

    if (currentHash.empty()) {
        std::cout << "No commits yet.\n";
        return;
    }

    while (!currentHash.empty()) {
        std::string commitPath = commitsPath + "/" + currentHash;
        std::ifstream in(commitPath);
        if (!in) {
            std::cerr << "Failed to read commit: " << currentHash << "\n";
            break;
        }

        std::cout << "\n=== Commit " << currentHash << " ===\n";

        std::string line;
        std::string parentHash;

        while (std::getline(in, line)) {
            if (line.rfind("Parent:", 0) == 0) {
                parentHash = line.substr(7);
                parentHash.erase(0, parentHash.find_first_not_of(" \t"));
            }
            std::cout << line << "\n";
        }

        currentHash = parentHash;
    }
}
void MiniGit::branch(const std::string& name) {
   }
void MiniGit::checkout(const std::string& branchName) {
    
}
void MiniGit::merge(const std::string& branchName) {
    }


void MiniGit::diff(const std::string& hash1, const std::string& hash2) {}
}
