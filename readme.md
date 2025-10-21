# 🧠 vaskesh — AI-Powered Terminal

`vaskesh` is a custom-built Linux shell (like `bash` or `zsh`) that integrates AI command interpretation.  
You can type natural language commands like **"list all files"** or **"print working directory"**, and vaskesh will interpret and execute them automatically using an integrated AI model (via Ollama).

---

## 🚀 Features

- 🧩 Built-in command parsing and execution engine  
- 🧠 AI-powered natural language command interpretation  
- 🧵 Background job management (run commands with `&`)  
- 📜 Command history support (using GNU `readline`)  
- 🔧 Easily extendable command handlers  
- 🧹 Signal handling for clean job termination  

---

## 🗂️ Project Structure

vaskesh/
├── include/
│ ├── ai_requests.h # Handles AI request logic
│ ├── executor.h # Command execution functions
│ ├── jobs.h # Background job management
│ ├── parser.h # Input parsing and tokenization
│
├── src/
│ ├── main.c # Entry point of the shell
│ ├── ai_requests.c # AI communication with Ollama
│ ├── executor.c # Built-in and system command executor
│ ├── jobs.c # Background/foreground job control
│ ├── parser.c # Command parsing logic
│
├── Makefile # Build configuration
├── .myshell_history # Command history file (auto-created)
└── README.md # Project documentation

yaml
Copy code

---

## 🧰 Requirements

Before building, ensure the following dependencies are installed:

### For Linux:
```bash
sudo apt install build-essential libreadline-dev libcurl4-openssl-dev
For macOS (using Homebrew):
bash
brew install readline curl
You’ll also need Ollama installed and running:

bash
brew install --cask ollama
ollama run llama3
🏗️ Build Instructions
Compile the project using make:

bash
Copy code
make
This will produce an executable called myshell.

If you make changes to the source files, simply run:

bash
Copy code
make clean && make
▶️ Running vaskesh
To start the shell:

bash
Copy code
./myshell
You’ll see a prompt like this:

shell
Copy code
vaskesh>
You can now:

Type normal shell commands like ls, pwd, cat file.txt

Or type natural language commands like:

show me files

print working directory

list all processes

vaskesh will send these natural commands to the AI model, interpret the response (e.g., pwd), and execute it.


vaskesh> list files in current folder
main.c  parser.c  jobs.c  README.md

vaskesh> sleep 10 &
[1] 12345
vaskesh>
🧠 AI Integration Details
The AI interface is handled in ai_requests.c, which sends requests to the Ollama local model API via libcurl.
You can configure which model to use (e.g., llama3:latest) directly in the code.

Example of a raw Ollama response:

json
Copy code
{"model":"llama3:latest","created_at":"...","response":"pwd","done":false}
{"model":"llama3:latest","created_at":"...","response":"","done":true}
The shell captures the "response" text, logs it for visibility, and executes it as a command.

🧹 Cleaning Up
To remove the compiled binary and temporary files:

bash
Copy code
make clean
📄 License
This project is open-source and free to modify for personal or educational purposes.

✨ Author
Emmanuel Hateka 
Focused on high-performance, detail-oriented software engineering.