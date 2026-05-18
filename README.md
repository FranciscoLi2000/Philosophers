# Philosophers
A C program that simulates the classic dining philosophers scenario (people sharing forks at a table) and times actions safely.

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Norminette](https://img.shields.io/badge/norminette-passing-brightgreen.svg)
![School](https://img.shields.io/badge/school-42%20Barcelona-000000.svg)

## Table of Contents
- [The Problem — Explained for a Human](#the-problem--explained-for-a-human)
- [Key Concepts](#key-concepts)
- [Project Architecture](#project-architecture)
- [Function Reference](#function-reference)
- [How to Compile and Run](#how-to-compile-and-run)
- [Edge Cases and How the Program Handles Them](#edge-cases-and-how-the-program-handles-them)
- [Testing Guide](#testing-guide)
- [Common Mistakes and How to Avoid Them](#common-mistakes-and-how-to-avoid-them)
- [Glossary](#glossary)
- [Author](#author)

## The Problem — Explained for a Human
Imagine a round table with philosophers. Each philosopher needs two forks to eat, but there is only one fork between each pair. They repeat the same routine: think, take forks, eat, sleep, and think again. If a philosopher goes too long without eating, they “die,” and the simulation stops.

This story models a real computing challenge: many activities happen at the same time and must share limited resources. That situation is called **concurrency**, and it is common in servers, games, and operating systems. The hard part is making sure everyone shares fairly and safely. If everyone grabs one fork and waits forever, we get a **deadlock** (a total standstill). If one philosopher is always unlucky and never eats, we get **starvation** (someone is blocked forever). And if two actions change the same data at the same time, we get a **data race** (unpredictable results).

A real-world analogy: imagine five people sharing two pens to sign forms. If everyone grabs one pen and waits for the second, no one can sign. If one person always gets the pens first, someone else never finishes. The program you run here is a safe, timed version of that same story.

## Key Concepts

### Thread
**Definition (plain English):** A thread is a small worker inside one program. Many threads can run at once, sharing the same memory.
**Real-world analogy:** Several cooks working in one kitchen. They share the same pantry but each cook has their own task.
**In this project:** In `philo/`, each philosopher is a thread that runs the routine (eat, sleep, think) in parallel with the others.

### Process
**Definition (plain English):** A process is one running copy of a program, with its own memory space.
**Real-world analogy:** A single restaurant branch with its own kitchen and staff.
**In this project:** In `philo_bonus/`, each philosopher is its own process created with `fork()` (a system call that duplicates the running program).

### Semaphore
**Definition (plain English):** A semaphore is a shared counter that limits how many people can use a resource at the same time.
**Real-world analogy:** Numbered tickets for a small concert hall; only people with tickets can enter.
**In this project:** The bonus version uses one semaphore for fork count and one for safe printing.

### Mutex
**Definition (plain English):** A mutex is a lock that allows only one thread to use a shared resource at a time. Others must wait until the lock is released.
**Real-world analogy:** A single key for a shared storage room. Whoever holds the key is the only one who can enter.
**In this project:** Each fork is protected by a mutex, and shared flags like “stop” are also protected to prevent conflicting reads and writes.

### Race condition
**Definition (plain English):** A race condition happens when the result depends on the timing of two or more actions that touch the same data.
**Real-world analogy:** Two people updating a whiteboard at the same time. The final message depends on who writes last.
**In this project:** Meal times and the global stop flag are shared. They are read and written under locks so the monitor sees correct data.

### Deadlock
**Definition (plain English):** Deadlock means everyone is waiting for a resource that will never become free.
**Real-world analogy:** A circle of people each holding one piece of a puzzle and refusing to swap until they get another.
**In this project:** Forks are taken in a consistent order (based on philosopher id) to avoid the “everyone grabs one fork” trap.

### Starvation
**Definition (plain English):** Starvation is when one participant never gets what they need, even though others keep progressing.
**Real-world analogy:** A line at a buffet where one person is always cut in front of.
**In this project:** The simulation checks how long each philosopher has gone without eating, and it stops if someone starves.

## Project Architecture

### File structure
```text
Philosophers/
├── .gitignore
├── LICENSE
├── Philosophers.pdf
├── README.md
├── philo/
│   ├── Makefile
│   ├── init.c
│   ├── main.c
│   ├── monitor.c
│   ├── philo.h
│   ├── routine.c
│   └── utils.c
└── philo_bonus/
    ├── Makefile
    ├── init.c
    ├── main.c
    ├── monitor.c
    ├── philo_bonus.h
    ├── routine.c
    └── utils.c
```

### File-by-file purpose
- `.gitignore` — Keeps build artifacts and editor files out of version control.
- `LICENSE` — Legal terms for using and sharing the project.
- `Philosophers.pdf` — The official project specification from 42.
- `README.md` — This teaching and usage guide.

**philo/**
- `philo/Makefile` — Builds the mandatory threaded version (`philo`).
- `philo/main.c` — Program entry point, thread creation, and cleanup.
- `philo/init.c` — Argument parsing, memory allocation, and mutex setup.
- `philo/monitor.c` — Watches for deaths or completion of meals.
- `philo/philo.h` — Shared structs and function prototypes.
- `philo/routine.c` — The philosopher routine (take forks, eat, sleep, think).
- `philo/utils.c` — Timing, parsing, stop checks, and safe printing.

**philo_bonus/**
- `philo_bonus/Makefile` — Builds the bonus process-based version (`philo_bonus`).
- `philo_bonus/main.c` — Program entry point, process creation, and cleanup.
- `philo_bonus/init.c` — Argument parsing, process ID (PID) allocation, and semaphore setup.
- `philo_bonus/monitor.c` — Per-process monitor (a watcher thread) that detects death.
- `philo_bonus/philo_bonus.h` — Shared structs, semaphore names, prototypes.
- `philo_bonus/routine.c` — The philosopher process routine and actions.
- `philo_bonus/utils.c` — Timing, parsing, and safe printing with semaphores.

### Data flow (from `main()` to the workers)
**Mandatory version (`philo/`):**
1. `main()` calls `init_rules()` to parse arguments and create mutexes.
2. `start_threads()` creates one thread per philosopher and sets the start time.
3. Each thread runs `philo_routine()` in a loop: take forks → eat → sleep → think.
4. The main thread runs `monitor_simulation()` and sets the stop flag on death or completion.
5. `join_threads()` waits for all threads, then `cleanup()` frees resources.

**Bonus version (`philo_bonus/`):**
1. `main()` calls `init_rules()` to parse arguments and open semaphores.
2. `start_processes()` forks one process per philosopher.
3. Each child runs `run_philo()`, which also starts a monitor thread.
4. Children exit with status `0` (finished) or `1` (death).
5. The parent waits in `wait_processes()` and terminates remaining children on a death.

### Thread lifecycle (mandatory version)
```text
[thread created]
      |
      v
 take_forks
      |
      v
    eat
      |
      v
   sleep
      |
      v
   think
      |
      +---- (loop until stop flag)
      |
      v
 [thread exits]
```

## Function Reference

---
##### `start_threads(t_rules *rules)`
**File:** `philo/main.c`
**Purpose:** Create one philosopher thread per seat and set the shared start time.
**Why it exists:** Keeps thread creation and failure handling in one place.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings and arrays. |
**Returns:** `0` on success, `1` if a thread could not be created.
**Concurrency note:** Threads begin running immediately after creation.
---
##### `join_threads(t_rules *rules)`
**File:** `philo/main.c`
**Purpose:** Wait for all philosopher threads to finish.
**Why it exists:** Prevents zombie threads and ensures clean shutdown.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings and arrays. |
**Returns:** Nothing (void).
---
##### `cleanup(t_rules *rules)`
**File:** `philo/main.c`
**Purpose:** Destroy mutexes and free allocated memory.
**Why it exists:** Avoids memory leaks and leaves no locked resources.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings and arrays. |
**Returns:** Nothing (void).
**Concurrency note:** Must run after threads are joined because it destroys mutexes.
---
##### `main(int argc, char **argv)`
**File:** `philo/main.c`
**Purpose:** Parse arguments, launch threads, and supervise the simulation.
**Why it exists:** It is the program entry point required by C.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| argc | `int` | Number of command-line arguments. |
| argv | `char **` | Argument strings. |
**Returns:** `0` on success, `1` on error.
---
##### `parse_args(t_rules *rules, int argc, char **argv)`
**File:** `philo/init.c`
**Purpose:** Validate command-line arguments and store them in `rules`.
**Why it exists:** Input validation is isolated and easier to test.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Destination for parsed values. |
| argc | `int` | Number of command-line arguments. |
| argv | `char **` | Argument strings. |
**Returns:** `0` on success, `1` if any argument is invalid.
---
##### `init_mutexes(t_rules *rules)`
**File:** `philo/init.c`
**Purpose:** Initialize all mutexes (forks, meal locks, state, and output).
**Why it exists:** Centralizes mutex setup for consistent cleanup.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings and arrays. |
**Returns:** `0` on success, `1` on failure.
**Concurrency note:** These locks protect shared resources during the run.
---
##### `init_philos(t_rules *rules)`
**File:** `philo/init.c`
**Purpose:** Fill each philosopher’s id, fork pointers, and counters.
**Why it exists:** Keeps per-philosopher setup separate from parsing logic.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings and arrays. |
**Returns:** Nothing (void).
---
##### `init_rules(t_rules *rules, int argc, char **argv)`
**File:** `philo/init.c`
**Purpose:** Parse arguments, allocate arrays, and prepare mutexes.
**Why it exists:** Provides a single initialization entry point for `main()`.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings to populate. |
| argc | `int` | Number of command-line arguments. |
| argv | `char **` | Argument strings. |
**Returns:** `0` on success, `1` on failure.
---
##### `check_death(t_rules *rules, t_philo *philo)`
**File:** `philo/monitor.c`
**Purpose:** Detect if a philosopher has exceeded `time_to_die`.
**Why it exists:** Keeps the death check readable and reusable in the monitor loop.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings. |
| philo | `t_philo *` | Philosopher to check. |
**Returns:** `1` if death is detected, `0` otherwise.
**Concurrency note:** Uses `meal_mutex` and `state_mutex` to read shared data safely.
---
##### `check_meals(t_rules *rules)`
**File:** `philo/monitor.c`
**Purpose:** Check if all philosophers have eaten enough times.
**Why it exists:** Isolates meal completion logic from the main loop.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings. |
**Returns:** `1` if everyone is full, `0` otherwise.
**Concurrency note:** Reads `meals_eaten` under each philosopher’s `meal_mutex`.
---
##### `monitor_simulation(t_rules *rules)`
**File:** `philo/monitor.c`
**Purpose:** Supervise the run and stop on death or completion.
**Why it exists:** The main thread needs one place to control simulation end.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings. |
**Returns:** `0` when monitoring ends.
**Concurrency note:** Reads shared state with mutex protection.
---
##### `take_forks(t_philo *philo)`
**File:** `philo/routine.c`
**Purpose:** Lock two fork mutexes in a safe order.
**Why it exists:** Centralizes fork ordering to avoid deadlock.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| philo | `t_philo *` | Philosopher performing the action. |
**Returns:** Nothing (void).
**Concurrency note:** Locks shared fork mutexes.
---
##### `eat(t_philo *philo)`
**File:** `philo/routine.c`
**Purpose:** Update meal data, print, and hold forks while eating.
**Why it exists:** Groups all meal-related updates in one place.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| philo | `t_philo *` | Philosopher performing the action. |
**Returns:** Nothing (void).
**Concurrency note:** Uses `meal_mutex` and shared fork mutexes.
---
##### `sleep_think(t_philo *philo)`
**File:** `philo/routine.c`
**Purpose:** Perform the sleeping and thinking phases.
**Why it exists:** Keeps the main routine loop readable.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| philo | `t_philo *` | Philosopher performing the action. |
**Returns:** Nothing (void).
**Concurrency note:** Calls `print_state` which uses shared output locks.
---
##### `philo_routine(void *arg)`
**File:** `philo/routine.c`
**Purpose:** Main loop executed by each philosopher thread.
**Why it exists:** Each philosopher must run independently and repeatedly.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| arg | `void *` | Pointer to the philosopher struct. |
**Returns:** `NULL` when the thread exits.
**Concurrency note:** Uses fork mutexes, meal mutex, and the shared stop flag.
---
##### `get_time_ms(void)`
**File:** `philo/utils.c`
**Purpose:** Get the current time in milliseconds.
**Why it exists:** The project uses millisecond timing for all actions.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| (none) | — | — |
**Returns:** Current time in milliseconds since the Unix epoch.
---
##### `simulation_stopped(t_rules *rules)`
**File:** `philo/utils.c`
**Purpose:** Check whether the simulation stop flag is set.
**Why it exists:** Centralizes safe access to the stop flag.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings. |
**Returns:** `1` if stopped, `0` otherwise.
**Concurrency note:** Uses `state_mutex` to read the flag safely.
---
##### `precise_sleep(t_rules *rules, long duration)`
**File:** `philo/utils.c`
**Purpose:** Sleep for a duration while still reacting to stop.
**Why it exists:** Prevents oversleeping and allows clean shutdowns.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings. |
| duration | `long` | Sleep time in milliseconds. |
**Returns:** `0` after sleeping or stopping.
**Concurrency note:** Checks the shared stop flag during the sleep.
---
##### `parse_positive(const char *str, long *out)`
**File:** `philo/utils.c`
**Purpose:** Convert a string into a positive integer with validation.
**Why it exists:** The project forbids using `atoi`, so a safe parser is needed.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| str | `const char *` | Input string to parse. |
| out | `long *` | Destination for the parsed number. |
**Returns:** `0` on success, `1` on invalid input.
---
##### `print_state(t_philo *philo, const char *msg, int force)`
**File:** `philo/utils.c`
**Purpose:** Print a philosopher state message with clean, ordered output.
**Why it exists:** Prevents interleaved logs and avoids printing after stop.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| philo | `t_philo *` | Philosopher to print for. |
| msg | `const char *` | Text message to print. |
| force | `int` | Print even after stop (used for death). |
**Returns:** `0` if printed, `1` if skipped.
**Concurrency note:** Uses `state_mutex` and `write_mutex` for safe output.
---
##### `start_processes(t_rules *rules)`
**File:** `philo_bonus/main.c`
**Purpose:** Fork one process per philosopher and store their PIDs.
**Why it exists:** Keeps all forking logic in one function.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings. |
**Returns:** `0` on success, `1` on failure.
**Concurrency note:** Each child process runs independently after `fork()`.
---
##### `wait_processes(t_rules *rules)`
**File:** `philo_bonus/main.c`
**Purpose:** Wait for child processes and stop all on a death.
**Why it exists:** The parent must reap children and enforce end conditions.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings. |
**Returns:** Nothing (void).
**Concurrency note:** Uses `waitpid` and `kill` to coordinate processes.
---
##### `cleanup(t_rules *rules)`
**File:** `philo_bonus/main.c`
**Purpose:** Close semaphores and free the PID array.
**Why it exists:** Avoids leaking OS resources after the run.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings. |
**Returns:** Nothing (void).
**Concurrency note:** Must run after processes are done because it closes semaphores.
---
##### `main(int argc, char **argv)`
**File:** `philo_bonus/main.c`
**Purpose:** Parse arguments, start processes, and wait for completion.
**Why it exists:** It is the program entry point required by C.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| argc | `int` | Number of command-line arguments. |
| argv | `char **` | Argument strings. |
**Returns:** `0` on success, `1` on error.
---
##### `parse_args(t_rules *rules, int argc, char **argv)`
**File:** `philo_bonus/init.c`
**Purpose:** Validate and store command-line arguments.
**Why it exists:** Keeps input parsing clear and reusable.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Destination for parsed values. |
| argc | `int` | Number of command-line arguments. |
| argv | `char **` | Argument strings. |
**Returns:** `0` on success, `1` on invalid input.
---
##### `open_sems(t_rules *rules)`
**File:** `philo_bonus/init.c`
**Purpose:** Create or open the named semaphores used by all processes.
**Why it exists:** Centralizes semaphore setup so cleanup is predictable.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings. |
**Returns:** `0` on success, `1` on failure.
**Concurrency note:** Semaphores are shared across all processes.
---
##### `allocate_pids(t_rules *rules)`
**File:** `philo_bonus/init.c`
**Purpose:** Allocate the PID array for child process tracking.
**Why it exists:** The parent must know all child PIDs to stop them.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings. |
**Returns:** `0` on success, `1` on failure.
---
##### `close_sems(t_rules *rules)`
**File:** `philo_bonus/init.c`
**Purpose:** Close and unlink the named semaphores.
**Why it exists:** Ensures no semaphore is left behind in the OS.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings. |
**Returns:** Nothing (void).
**Concurrency note:** Stops shared synchronization resources.
---
##### `init_rules(t_rules *rules, int argc, char **argv)`
**File:** `philo_bonus/init.c`
**Purpose:** Parse arguments, allocate PIDs, and open semaphores.
**Why it exists:** Provides a single initialization entry point for `main()`.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings to populate. |
| argc | `int` | Number of command-line arguments. |
| argv | `char **` | Argument strings. |
**Returns:** `0` on success, `1` on failure.
---
##### `announce_death(t_philo *philo, long now)`
**File:** `philo_bonus/monitor.c`
**Purpose:** Print a death message and block further output.
**Why it exists:** Ensures only one clean death line is printed.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| philo | `t_philo *` | Philosopher who died. |
| now | `long` | Current time in milliseconds. |
**Returns:** Nothing (void).
**Concurrency note:** Locks the shared write semaphore and keeps it locked.
---
##### `death_monitor(void *arg)`
**File:** `philo_bonus/monitor.c`
**Purpose:** Monitor a philosopher process and exit if it starves.
**Why it exists:** Each process needs a dedicated watcher for death.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| arg | `void *` | Pointer to the philosopher struct. |
**Returns:** `NULL` (never reached; exits the process).
**Concurrency note:** Reads `last_meal` under `meal_mutex`.
---
##### `take_forks(t_philo *philo)`
**File:** `philo_bonus/routine.c`
**Purpose:** Reserve two forks from the shared semaphore.
**Why it exists:** Keeps fork acquisition logic in one function.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| philo | `t_philo *` | Philosopher performing the action. |
**Returns:** Nothing (void).
**Concurrency note:** Uses the shared forks semaphore.
---
##### `eat(t_philo *philo)`
**File:** `philo_bonus/routine.c`
**Purpose:** Update meal data, print, and hold forks while eating.
**Why it exists:** Groups all meal-related updates in one place.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| philo | `t_philo *` | Philosopher performing the action. |
**Returns:** Nothing (void).
**Concurrency note:** Uses `meal_mutex` and the forks semaphore.
---
##### `sleep_think(t_philo *philo)`
**File:** `philo_bonus/routine.c`
**Purpose:** Perform the sleeping and thinking phases.
**Why it exists:** Keeps the main loop readable.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| philo | `t_philo *` | Philosopher performing the action. |
**Returns:** Nothing (void).
**Concurrency note:** Calls `print_state`, which uses the write semaphore.
---
##### `run_philo(t_rules *rules, int id)`
**File:** `philo_bonus/routine.c`
**Purpose:** Run the philosopher loop inside a child process.
**Why it exists:** Each child must perform its own routine after `fork()`.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| rules | `t_rules *` | Shared simulation settings. |
| id | `int` | Philosopher id (1..N). |
**Returns:** Nothing (void) because the process exits on completion or death.
**Concurrency note:** Starts a monitor thread and uses mutexes and semaphores.
---
##### `get_time_ms(void)`
**File:** `philo_bonus/utils.c`
**Purpose:** Get the current time in milliseconds.
**Why it exists:** The simulation uses millisecond timing.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| (none) | — | — |
**Returns:** Current time in milliseconds since the Unix epoch.
---
##### `precise_sleep(long duration)`
**File:** `philo_bonus/utils.c`
**Purpose:** Sleep in small steps for accurate timing.
**Why it exists:** Prevents long oversleeping from `usleep`.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| duration | `long` | Sleep time in milliseconds. |
**Returns:** Nothing (void).
---
##### `parse_positive(const char *str, long *out)`
**File:** `philo_bonus/utils.c`
**Purpose:** Convert a string into a positive integer with validation.
**Why it exists:** The project forbids using `atoi`, so a safe parser is needed.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| str | `const char *` | Input string to parse. |
| out | `long *` | Destination for the parsed number. |
**Returns:** `0` on success, `1` on invalid input.
---
##### `print_state(t_philo *philo, const char *msg)`
**File:** `philo_bonus/utils.c`
**Purpose:** Print a philosopher state message safely.
**Why it exists:** Prevents interleaved output from multiple processes.
**Parameters:**
| Parameter | Type | Meaning |
|---|---|---|
| philo | `t_philo *` | Philosopher to print for. |
| msg | `const char *` | Text message to print. |
**Returns:** Nothing (void).
**Concurrency note:** Uses the shared write semaphore.
---

## How to Compile and Run
```bash
# Clone and compile (mandatory version)
git clone https://github.com/FranciscoLi2000/Philosophers.git
cd Philosophers/philo
make

# Basic usage
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep
./philo 5 800 200 200

# With optional argument
./philo 5 800 200 200 7
```

**Arguments (one sentence each):**
- `number_of_philosophers`: How many philosophers (and forks) are at the table.
- `time_to_die`: Maximum time in milliseconds a philosopher can go without eating.
- `time_to_eat`: Time in milliseconds each eating action lasts.
- `time_to_sleep`: Time in milliseconds each sleeping action lasts.
- `number_of_times_each_philosopher_must_eat` (optional): Stop the simulation after everyone eats this many times.

**Bonus version build:**
```bash
cd ../philo_bonus
make
./philo_bonus 5 800 200 200 7
```

## Edge Cases and How the Program Handles Them
- **1 philosopher:** The single philosopher can take only one fork, waits, and dies when `time_to_die` is reached (handled explicitly in both versions).
- **A philosopher dies before anyone eats:** The monitor detects the missed deadline and stops the simulation immediately.
- **All philosophers eat the required number of times:** The monitor (or parent process) ends the simulation cleanly once everyone reaches the limit.
- **Invalid arguments (negative, non-integer, too few):** The parser rejects them and prints `Error` without starting the simulation.

## Testing Guide
```bash
./philo 1 800 200 200     # Expected: philosopher dies, program exits
./philo 5 800 200 200     # Expected: no one dies, runs indefinitely
./philo 4 410 200 200     # Expected: runs without deaths
./philo 4 310 200 100     # Expected: a philosopher may die
./philo 5 800 200 200 7   # Expected: stops after each philosopher eats 7 times
```

## Common Mistakes and How to Avoid Them
1. **Mistake:** Taking forks in the same order for everyone.
   **Why it’s wrong:** This can create a circular wait and deadlock.
   **How this implementation avoids it:** Fork order changes by philosopher id (even/odd).

2. **Mistake:** Printing without synchronization.
   **Why it’s wrong:** Output lines get mixed and become unreadable.
   **How this implementation avoids it:** A dedicated mutex/semaphore guards all prints.

3. **Mistake:** Ignoring the single philosopher case.
   **Why it’s wrong:** A single philosopher can never get two forks, so the program can hang or behave incorrectly.
   **How this implementation avoids it:** A special case handles one philosopher and lets the monitor detect death.

4. **Mistake:** Letting threads run after a death.
   **Why it’s wrong:** The simulation should stop immediately after the first death.
   **How this implementation avoids it:** A shared stop flag (or exit code) ends all workers.

5. **Mistake:** Oversleeping and missing deadlines.
   **Why it’s wrong:** Large `usleep` calls can delay death checks.
   **How this implementation avoids it:** `precise_sleep` sleeps in small steps and checks for stop.

## Glossary
| Term | Definition |
|------|------------|
| concurrency | Many activities progressing at the same time, within one program or across multiple processes. |
| thread | A small worker inside a process that shares memory with other threads. |
| process | A separate program instance with its own memory space. |
| mutex | A lock that allows only one thread to access a shared resource at a time. |
| semaphore | A shared counter used to limit access to a resource across processes. |
| race condition | A bug where timing changes the result because data is shared unsafely. |
| deadlock | A situation where everyone is waiting and no one can move forward. |
| starvation | A situation where one participant never gets the resource they need. |
| fork (resource) | The shared utensil each philosopher needs to eat; in code it is a locked resource. |
| monitor | A watcher that checks for death or completion conditions. |

## Author
Yufeng Li — 42 intra login: FranciscoLi2000 — [GitHub](https://github.com/FranciscoLi2000)
