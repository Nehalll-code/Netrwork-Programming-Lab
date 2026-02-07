# Network-Programming-Lab

Each lab directory contains:
- `server.c` – Server-side program
- `client.c` – Client-side program
- Supporting files (text files, databases, etc.)

---

## 🛠 System Requirements

- Operating System: Linux / UNIX
- Compiler: `gcc`
- Tools:
  - `make` (optional)
  - `git` (for version control)

Install required tools:
```bash
sudo apt update
sudo apt install gcc git

▶️ How to Compile and Run
Compile
gcc server.c -o server
gcc client.c -o client

Run Server (Terminal 1)
./server

Run Client (Terminal 2)
./client


⚠️ Always start the server before the client.
