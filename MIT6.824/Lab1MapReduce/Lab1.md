# Go

You'll implement all the labs in [Go](http://www.golang.org/). The Go web site contains lots of tutorial information. We will grade your labs using Go version 1.15; you should use 1.15 too. You can check your Go version by running `go version`.

We recommend that you work on the labs on your own machine, so you can use the tools, text editors, etc. that you are already familiar with. Alternatively, you can work on the labs on Athena.

#### macOS

You can use [Homebrew](https://brew.sh/) to install Go. After installing Homebrew, run `brew install go`.

#### Linux

Depending on your Linux distribution, you might be able to get an up-to-date version of Go from the package repository, e.g. by running `apt install golang`. Otherwise, you can manually install a binary from Go's website. First, make sure that you're running a 64-bit kernel (`uname -a` should mention "x86_64 GNU/Linux"), and then run:

```
$ wget -qO- https://golang.org/dl/go1.15.8.linux-amd64.tar.gz | sudo tar xz -C /usr/local
```

You'll need to make sure `/usr/local/go/bin` is on your `PATH`. You can do this by adding `export PATH=$PATH:/usr/local/go/bin` to your shell's init file ( commonly this is one of `.bashrc`, `.bash_profile` or `.zshrc`)

#### Windows

The labs probably won't work directly on Windows. If you're feeling adventurous, you can try to get them running inside [Windows Subsystem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install-win10) and following the Linux instructions above. Otherwise, you can fall back to Athena.

#### Athena

You can log into a public Athena host with `ssh {your kerberos}@athena.dialup.mit.edu`. Once you're logged in, to get Go 1.15, run:

```
$ setup ggo
```

##### Troubleshooting Athena

If you get the following error:

```
bash: setup: command not found
```

Then you might have removed something important from your shell's startup scripts. Compare your `~/.bashrc` to `/usr/prototype_user/.bashrc` and add anything that you're missing.

Note that if you use a shell besides bash on Athena, the 'setup' command might not work properly, and you might need to add `/mit/ggo/current/bin/` to your path manually.s

# 

# Lab guidance

## Hardness of assignments

Each task indicates how difficult it is:

- Easy: A few hours.
- Moderate: ~ 6 hours (per week).
- Hard: More than 6 hours (per week). If you start late, your solution is unlikely to pass all tests.

These times are rough estimates of our expectations.

The tasks in general require not many lines of code (a few hundred lines), but the code is conceptually complicated and often details matter a lot. Some of the tests are difficult to pass.

Don't start a lab the night before it is due; it much more time efficient to do the labs in several sessions spread over multiple days. Tracking down bugs in distributed system code is difficult, because of concurrency, crashes, and an unreliable network. It requires thought and careful debugging to understand and fix.

## Development/Debugging tips

- Learn Go. Do [Online Go tutorial](http://tour.golang.org/) and consult [How to write Go code](https://golang.org/doc/effective_go.html). See [Editors](https://golang.org/doc/editors.html) to set up your editor for Go.
- Use Go's [race detector](https://blog.golang.org/race-detector), with `go build -race` and `go run -race`. Fix any races it reports.
- Read [guide](https://thesquareplanet.com/blog/students-guide-to-raft/) for Raft specific advice.
- Advice on [locking](https://pdos.csail.mit.edu/6.824/labs/raft-locking.txt) in labs.
- Advice on [structuring](https://pdos.csail.mit.edu/6.824/labs/raft-structure.txt) lab software.
- [Diagram of Raft interactions](https://pdos.csail.mit.edu/6.824/notes/raft_diagram.pdf) to understand code flow between different parts of the system.
- A good way to debug your code is to insert print statements when a peer sends or receives a message, and collect the output in a file with `go test -race > out`. Then, by studying the trace of messages in the `out` file, you can identify where your implementation deviates from the desired protocol.
- You might want to structure your debug message in a specific format so that you can use `grep` to search easily for specific lines in `out`.
- You might find `DPrintf` useful instead of calling `log.Printf` directly to turn printing on and off as you debug different problems.
- You can use formatting options like colors or columns to make understanding the log output easier. [This post](https://blog.josejg.com/debugging-pretty/) goes over how to build a tool that labels logs with topics and then pretty prints the logs using colorized terminal output with separate columns for each server.
- To learn more about git, look at the [Pro Git book](https://git-scm.com/book/en/v2) or the [git user's manual](http://www.kernel.org/pub/software/scm/git/docs/user-manual.html).

It is well worth the time to follow these tips.

## Tips for debugging as a formal process

(This section is new! Feel free to contribute suggestions and ask for clarification.)

Debugging complex concurrent systems is not magic. Debugging is a science and a skill, and it can be learned and practiced. When done well, debugging is a process that will lead you inerringly from an initial observation of an error back to the exact fault that caused that error. In general, most bugs in complex systems cannot be solved using ad-hoc or "guess-and-check" approaches; you are likely to have the most success by being as methodical in your approach as possible.

Helpful definitions:

- A **fault** is an underlying cause of a bug. This is the mistake in a piece of code that allows for something to go wrong. They can be anything from a typo to a fundamental misunderstanding of a protocol that is enshrined in your code.

- An

   

  error

   

  is a deviation at a particular point in time between the theoretical correct state of a program and the actual state of the program at that time. An error is either a latent error, an observable error, or a masked error.

  - A **latent error** is not visible to you; it is silently propagating within your code. It will eventually become an observable error or a masked error.
  - An **observable error** is visible; it has been **surfaced** in the observable output of the program, either as an error message, an unexpected output, or the lack of an expected output.
  - A **masked error** is where wherein some property of the implementation or design allows a previous latent error to be ignored. For example, if a log entry is incorrectly added in Raft (a latent error), but then overwritten later by a correct entry before it is reported anywhere, the original error has been masked and can never become observable.

- **Instrumentation** refers to the parts of your code that report on the current state of your program. This includes both code to explicitly detect particular possible errors, and code to print out state so that it can later be manually examined to see whether or not it indicates the presence of any errors.

You can debug forwards (from fault to error), or backwards (from error to fault). The forward approach tends to devolve into a "guess-and-check" approach, and frequently does not allow for consistent or repeatable progress. By contrast, the backwards approach is always applicable to the 6.824 labs, and allows for making consistent progress, so we will focus on it here.

The backwards approach is always applicable for our labs because you always have an observable error to start with: the test cases themselves function as a form of instrumentation, which will detect certain kinds of errors and surface them to you.

### Working backwards to find an error

The execution of a program that you are debugging can generally be to have three phases:

1. The phase in which the execution of the program is correct and error-free. A fault may exist in the program, but it has not yet been manifested as an actual error.
2. The phase in which a fault has manifested, and has produced one or more latent errors in the program state. These are not yet visible, and may silently propagate within the program state.
3. The phase in which the latent errors have surfaced, and become observable errors. At this point, you have an indication that there's something wrong with the running program.

The goal of debugging is to narrow in on the exact location of the fault within your implementation. This means you need to identify the point in time when the fault is manifested, and the program moves from phase 1 to phase 2. It's hard to find the exact extent of phase 1, because it is generally prohibitively difficult to analyze the complete state of a program in sufficient detail to be certain that a fault has not yet manifested itself at any point in time.

Instead, the best approach is usually to work backwards and narrow down the size of phase 2 until it is as small as possible, so that the location of the fault is readily apparent. This is done by expanding the instrumentation of your code to surface errors sooner, and thereby spend less time in phase 2. This generally involves adding additional debugging statements and/or assertions to your code.

### Making progress

When adding instrumentation, you want to focus on making clear and deliberate progress at narrowing down the cause of the fault. You should identify the first observable error you can in your debugging output, and then attempt to narrow down the most proximate cause of that error. This is done by forming a hypothesis about what the most proximate cause of the error could be, and then adding instrumentation to test that hypothesis. If your hypothesis is true, then you have a new "first observable error" and can repeat the process. If false, then you must come up with a new hypothesis about the proximate cause and test that instead.

It's important to maintain a sense of exactly what your current first observable error is, as you move backwards, so that you don't get lost among other errors that may be observable in your output.

At the beginning, you don't know anything about where the fault could be; it could be anywhere in your entire program. But as you advance, the possible interval in which the fault could be will narrow, until you reach a single line of code that must contain the fault. When you're down to the execution of a single function or a single block of code, it can be helpful to use a "bisection" approach, where you repeatedly add instrumentation halfway through the interval, and narrow down to one half or the other. (This is like binary search, but for finding bugs.)

### Adding instrumentation

As a debugger, your main challenge is to pick the best locations for your instrumentation to best narrow down the location of your fault, along with deciding the most useful pieces of information to report in that piece of instrumentation. You can and should use your knowledge of your implementation to speed this up, but you must always check your assumptions by actually running your code and observing the results of your instrumentation.

Because instrumentation is so essential to your debugging process, you should take the time to design and implement it carefully. Consider some of the following questions when designing your instrumentation:

- How much detail do you need from your instrumentation? Either in general, or just for the current step in your debugging process? How can you make it easier to adjust the level of detail and the main focus of your instrumentation? Can you turn on or off different pieces of debugging without deleting them from your code?

  (In particular, consider using an approach like the provided DPrintf function does, and defining one or more constant boolean flags to turn on or off different aspects of your instrumentation.)

- How can you optimize your own ability to quickly read and understand what your instrumentation is indicating? Can you use text colors, columns, consistent formats, codewords, or symbols to make it easier to read?

  The best approach will be personalized to the particular way that YOU best percieve information, so you should experiment to find out what works well for you.

- How can you enhance your own ability to add instrumentation? Can you use existing tools (like the go "log" package) to help? (I recommend turning on the Lmicroseconds flag, if you do.)

  Can you build your own helper functions, so that a common set of data (current server, term, and role, perhaps?) will always be displayed?

One specific note: make sure to learn about format strings if you aren't already familiar with them. You can refer to the [Wikipedia article on Printf format strings](https://en.wikipedia.org/wiki/Printf_format_string), or to the [Go-specific documentation](https://golang.org/pkg/fmt/). You will likely be much happier using functions like 'log.Printf' or 'fmt.Printf' than trying to achieve the same effects with multiple print functions.

You might also consider trying to condense each individual event you report into a single line to facilitate your ability to scan output quickly.

### Tips on asking for help

As mentioned above, debugging is a complex and iterative process. It can be hard for TAs to help you debug your code over Piazza; we strongly encourage you to bring your difficult debugging challenges to Office Hours instead! We will be happy to help walk you through the debugging approaches that are best applicable to your situation. Of course, you are likely to get the most mileage out of your time in Office Hours if you make a significant attempt at debugging the problem yourself in as methodical a manner as possible.

### Tips on timeouts

It should be noted that tweaking timeouts rarely fixes bugs, and that doing so should be a last resort. We frequently see students willing to keep making arbitrary tweaks to their code (especially timeouts) rather than following a careful debugging process. Doing this is a great way to obscure underlying bugs by masking them instead of fixing them; they will often still show up in rare cases, even if they appear fixed in the common case.

In particular, in Raft, there are wide ranges of timeouts that will let your code work. While you CAN pick bad timeout values, it won't take much time to find timeouts that are functional.

### Additional tips

- It's worth noting that there may be multiple faults in your code! It is often easiest to narrow down a bug by focusing on the first fault, rather than the last, because then there will be fewer errors in the program state to consider.
- Try to avoid ruling out a bug in one piece of code simply because you believe that it's correct. If you're dealing with a bug, that usually implies that there's something wrong with your mental model of your code -- either how your implementation actually works, or else your understanding of how it's supposed to work. As such, you can't rely too much on your mental model; always verify your assumptions.
- Debugging is a complex and multifaceted skill, and doing it well takes discipline and clear consideration. Try to think carefully about why your debugging approaches are succeeding or failing (or even just being convenient or painful), and take the time to find and explore new approaches that may help you. Time invested now in improving your debugging knowledge will pay off well in later labs.
- Making premature fixes are often dangerous when debugging. They may or may not solve the issue you're locking at, and are just as likely to shift the exact location and presentation of the fault you're trying to track down. Even worse, they may simply mask the real fault, rather than solve it. Wait until you're confident about the exact fault causing your observable error before you attempt a fix.
- Sometimes, making a fix doesn't immediately solve all of your problems. Sometimes, the same test case still fails, because the old fault was corrected, and a new fault that was previously hidden is now visible.
- Don't neglect the possibility of bugs in your glue code. Elements like main loops, locks, and channel I/O may seem simple and unlikely to be wrong, but they can sometimes hide extremely challenging bugs.
- When possible, consider writing your code to "fail loudly". Instead of trying to tolerate unexpected states, try to explicitly detect states that should never be allowed to happen, and immediately report these errors. Consider even immediately calling the Go 'panic' function in these cases to fail especially loudly. See also the Wikipedia page on [Offensive programming](https://en.wikipedia.org/wiki/Offensive_programming) techniques. Remember that the longer you allow errors to remain latent, the longer it will take to narrow down the true underlying fault.
- When dealing with a bug that occurs sporadically, the best approach is usually to log aggressively and dump the output to a file. It's easier to filter out irrelevant parts of a verbose log (such as with a separate script) than it is to wait for the error to reappear after N runs.
- When you're failing a test, and it's not obvious why, it's usually worth taking the time to understand what the test is actually doing, and which part of the test is observing the problem. It can be helpful to add print statements to the test code so that you know when events are happening.
- And one last note: locking strategy matters. A lack of races reported by the race detector does not indicate that your locking strategy is correct. In particular, fine-grained locks can be dangerous, because they can introduce more changes for interleaving.

Happy debugging!

# Introduction

### Introduction

In this lab you'll build a MapReduce system. You'll implement a worker process that calls application Map and Reduce functions and handles reading and writing files, and a coordinator process that hands out tasks to workers and copes with failed workers. You'll be building something similar to the [MapReduce paper](http://research.google.com/archive/mapreduce-osdi04.pdf). (Note: the lab uses "coordinator" instead of the paper's "master".)

### Getting started

You need to [setup Go](https://pdos.csail.mit.edu/6.824/labs/go.html) to the labs.

You'll fetch the initial lab software with [git](https://git-scm.com/) (a version control system). To learn more about git, look at the [Pro Git book](https://git-scm.com/book/en/v2) or the [git user's manual](http://www.kernel.org/pub/software/scm/git/docs/user-manual.html). To fetch the 6.824 lab software:

```
$ git clone git://g.csail.mit.edu/6.824-golabs-2021 6.824
$ cd 6.824
$ ls
Makefile src
$
```

We supply you with a simple sequential mapreduce implementation in `src/main/mrsequential.go`. It runs the maps and reduces one at a time, in a single process. We also provide you with a couple of MapReduce applications: word-count in `mrapps/wc.go`, and a text indexer in `mrapps/indexer.go`. You can run word count sequentially as follows:

```
$ cd ~/6.824
$ cd src/main
$ go build -race -buildmode=plugin ../mrapps/wc.go
$ rm mr-out*
$ go run -race mrsequential.go wc.so pg*.txt
$ more mr-out-0
A 509
ABOUT 2
ACT 8
...
```

(Note: If you don't compile with `-race`, you won't be able to run with `-race`)

`mrsequential.go` leaves its output in the file `mr-out-0`. The input is from the text files named `pg-xxx.txt`.

Feel free to borrow code from `mrsequential.go`. You should also have a look at `mrapps/wc.go` to see what MapReduce application code looks like.

### Your Job ([moderate/hard](https://pdos.csail.mit.edu/6.824/labs/guidance.html))

Your job is to implement a distributed MapReduce, consisting of two programs, the coordinator and the worker. There will be just one coordinator process, and one or more worker processes executing in parallel. In a real system the workers would run on a bunch of different machines, but for this lab you'll run them all on a single machine. The workers will talk to the coordinator via RPC. Each worker process will ask the coordinator for a task, read the task's input from one or more files, execute the task, and write the task's output to one or more files. The coordinator should notice if a worker hasn't completed its task in a reasonable amount of time (for this lab, use ten seconds), and give the same task to a different worker.

We have given you a little code to start you off. The "main" routines for the coordinator and worker are in `main/mrcoordinator.go` and `main/mrworker.go`; don't change these files. You should put your implementation in `mr/coordinator.go`, `mr/worker.go`, and `mr/rpc.go`.

Here's how to run your code on the word-count MapReduce application. First, make sure the word-count plugin is freshly built:

```
$ go build -race -buildmode=plugin ../mrapps/wc.go
```

In the `main` directory, run the coordinator.

```
$ rm mr-out*
$ go run -race mrcoordinator.go pg-*.txt
```

The `pg-*.txt` arguments to `mrcoordinator.go` are the input files; each file corresponds to one "split", and is the input to one Map task. The `-race` flags runs go with its race detector.

In one or more other windows, run some workers:

```
$ go run -race mrworker.go wc.so
```

When the workers and coordinator have finished, look at the output in `mr-out-*`. When you've completed the lab, the sorted union of the output files should match the sequential output, like this:

```
$ cat mr-out-* | sort | more
A 509
ABOUT 2
ACT 8
...
```

We supply you with a test script in `main/test-mr.sh`. The tests check that the `wc` and `indexer` MapReduce applications produce the correct output when given the `pg-xxx.txt` files as input. The tests also check that your implementation runs the Map and Reduce tasks in parallel, and that your implementation recovers from workers that crash while running tasks.

If you run the test script now, it will hang because the coordinator never finishes:

```
$ cd ~/6.824/src/main
$ bash test-mr.sh
*** Starting wc test.
```

You can change `ret := false` to true in the Done function in `mr/coordinator.go` so that the coordinator exits immediately. Then:

```
$ bash test-mr.sh
*** Starting wc test.
sort: No such file or directory
cmp: EOF on mr-wc-all
--- wc output is not the same as mr-correct-wc.txt
--- wc test: FAIL
$
```

The test script expects to see output in files named `mr-out-X`, one for each reduce task. The empty implementations of `mr/coordinator.go` and `mr/worker.go` don't produce those files (or do much of anything else), so the test fails.

When you've finished, the test script output should look like this:

```
$ bash test-mr.sh
*** Starting wc test.
--- wc test: PASS
*** Starting indexer test.
--- indexer test: PASS
*** Starting map parallelism test.
--- map parallelism test: PASS
*** Starting reduce parallelism test.
--- reduce parallelism test: PASS
*** Starting crash test.
--- crash test: PASS
*** PASSED ALL TESTS
$
```

You'll also see some errors from the Go RPC package that look like

```
2019/12/16 13:27:09 rpc.Register: method "Done" has 1 input parameters; needs exactly three
```

Ignore these messages; registering the coordinator as an [RPC server](https://golang.org/src/net/rpc/server.go) checks if all its methods are suitable for RPCs (have 3 inputs); we know that `Done` is not called via RPC.



### A few rules:

- The map phase should divide the intermediate keys into buckets for `nReduce` reduce tasks, where `nReduce` is the argument that `main/mrcoordinator.go` passes to `MakeCoordinator()`.
- The worker implementation should put the output of the X'th reduce task in the file `mr-out-X`.
- A `mr-out-X` file should contain one line per Reduce function output. The line should be generated with the Go `"%v %v"` format, called with the key and value. Have a look in `main/mrsequential.go` for the line commented "this is the correct format". The test script will fail if your implementation deviates too much from this format.
- You can modify `mr/worker.go`, `mr/coordinator.go`, and `mr/rpc.go`. You can temporarily modify other files for testing, but make sure your code works with the original versions; we'll test with the original versions.
- The worker should put intermediate Map output in files in the current directory, where your worker can later read them as input to Reduce tasks.
- `main/mrcoordinator.go` expects `mr/coordinator.go` to implement a `Done()` method that returns true when the MapReduce job is completely finished; at that point, `mrcoordinator.go` will exit.
- When the job is completely finished, the worker processes should exit. A simple way to implement this is to use the return value from `call()`: if the worker fails to contact the coordinator, it can assume that the coordinator has exited because the job is done, and so the worker can terminate too. Depending on your design, you might also find it helpful to have a "please exit" pseudo-task that the coordinator can give to workers.

### Hints

- One way to get started is to modify `mr/worker.go`'s `Worker()` to send an RPC to the coordinator asking for a task. Then modify the coordinator to respond with the file name of an as-yet-unstarted map task. Then modify the worker to read that file and call the application Map function, as in `mrsequential.go`.

- The application Map and Reduce functions are loaded at run-time using the Go plugin package, from files whose names end in `.so`.

- If you change anything in the `mr/` directory, you will probably have to re-build any MapReduce plugins you use, with something like `go build -race -buildmode=plugin ../mrapps/wc.go`

- This lab relies on the workers sharing a file system. That's straightforward when all workers run on the same machine, but would require a global filesystem like GFS if the workers ran on different machines.

- A reasonable naming convention for intermediate files is `mr-X-Y`, where X is the Map task number, and Y is the reduce task number.

- The worker's map task code will need a way to store intermediate key/value pairs in files in a way that can be correctly read back during reduce tasks. One possibility is to use Go's

   

  encoding/json

   

  package. To write key/value pairs to a JSON file:

  ```
    enc := json.NewEncoder(file)
    for _, kv := ... {
      err := enc.Encode(&kv)
  ```

  and to read such a file back:

  ```
    dec := json.NewDecoder(file)
    for {
      var kv KeyValue
      if err := dec.Decode(&kv); err != nil {
        break
      }
      kva = append(kva, kv)
    }
  ```

- The map part of your worker can use the `ihash(key)` function (in `worker.go`) to pick the reduce task for a given key.

- You can steal some code from `mrsequential.go` for reading Map input files, for sorting intermedate key/value pairs between the Map and Reduce, and for storing Reduce output in files.

- The coordinator, as an RPC server, will be concurrent; don't forget to lock shared data.

- Use Go's race detector, with `go build -race` and `go run -race`. `test-mr.sh` by default runs the tests with the race detector.

- Workers will sometimes need to wait, e.g. reduces can't start until the last map has finished. One possibility is for workers to periodically ask the coordinator for work, sleeping with `time.Sleep()` between each request. Another possibility is for the relevant RPC handler in the coordinator to have a loop that waits, either with `time.Sleep()` or `sync.Cond`. Go runs the handler for each RPC in its own thread, so the fact that one handler is waiting won't prevent the coordinator from processing other RPCs.

- The coordinator can't reliably distinguish between crashed workers, workers that are alive but have stalled for some reason, and workers that are executing but too slowly to be useful. The best you can do is have the coordinator wait for some amount of time, and then give up and re-issue the task to a different worker. For this lab, have the coordinator wait for ten seconds; after that the coordinator should assume the worker has died (of course, it might not have).

- If you choose to implement Backup Tasks (Section 3.6), note that we test that your code doesn't schedule extraneous tasks when workers execute tasks without crashing. Backup tasks should only be scheduled after some relatively long period of time (e.g., 10s).

- To test crash recovery, you can use the `mrapps/crash.go` application plugin. It randomly exits in the Map and Reduce functions.

- To ensure that nobody observes partially written files in the presence of crashes, the MapReduce paper mentions the trick of using a temporary file and atomically renaming it once it is completely written. You can use `ioutil.TempFile` to create a temporary file and `os.Rename` to atomically rename it.

- `test-mr.sh` runs all the processes in the sub-directory `mr-tmp`, so if something goes wrong and you want to look at intermediate or output files, look there. You can modify `test-mr.sh` to `exit` after the failing test, so the script does not continue testing (and overwrite the output files).

- `test-mr-many.sh` provides a bare-bones script for running `test-mr.sh` with a timeout (which is how we'll test your code). It takes as an argument the number of times to run the tests. You should not run several `test-mr.sh` instances in parallel because the coordinator will reuse the same socket, causing conflicts.







### No-credit challenge exercises

Implement your own MapReduce application (see examples in `mrapps/*`), e.g., Distributed Grep (Section 2.3 of the MapReduce paper).

Get your MapReduce coordinator and workers to run on separate machines, as they would in practice. You will need to set up your RPCs to communicate over TCP/IP instead of than Unix sockets (see commented out line in `Coordinator.server()`), and read/write files using a shared file system. For example, you can `ssh` into multiple [Athena cluster](http://kb.mit.edu/confluence/display/istcontrib/Getting+Started+with+Athena) machines at MIT, which use [AFS](http://kb.mit.edu/confluence/display/istcontrib/AFS+at+MIT+-+An+Introduction) to share files; or you could rent a couple AWS instances and use [S3](https://aws.amazon.com/s3/) for storage.



# 任务

实现MapReduce，由两个程序组成，`coordinator`和`worker`，其中将会只有一个`coordinator`和多个`worker`并发执行。在实际系统上，`worker`会在多个机器上运行，而这个Lab只在单个机器运行它们。

`worker`通过`RPC`与`coordinator`通信。每个`worker`会向`coodinator`请求分配任务。

`coordinator`需要注意哪个`worker`没有在一定时间内完成任务，在Lab中设置为10s，如果超市则将该任务分配给其他`worker`。

## 如何测试程序

在main目录下运行`coordinator`，并使用`-race`标志运行 race detector

然后开启多个窗口运行`worker`

当所有任务运行完成后查看输出，所有文件经排序后应该和Lab给我们的相似

- **一些可忽略的错误**

  You'll also see some errors from the Go RPC package that look like

  ```
  2019/12/16 13:27:09 rpc.Register: method "Done" has 1 input parameters; needs exactly three
  ```

  Ignore these messages; registering the coordinator as an [RPC server](https://golang.org/src/net/rpc/server.go) checks if all its  methods are suitable for RPCs (have 3 inputs); we know that `Done` is not called via RPC.

- **一些规则**

  - The map phase should divide the intermediate keys into buckets for `nReduce` reduce tasks, where `nReduce` is the argument that `main/mrcoordinator.go` passes to `MakeCoordinator()`.**Map需要把中间键值对划分为R 块**
  
  - The worker implementation should put the output of the X'th reduce task in the file `mr-out-X`.**输出文件名**
  
  - A `mr-out-X` file should contain one line per Reduce function output. The line should be generated with the Go `"%v %v"` format, called with the key and value. Have a look in `main/mrsequential.go` for the line commented "this is the correct format". The test script will fail if your implementation deviates too much from this format.**每个Reduce调用在输出文件中占一行，并且有一定输出格式，如果不按格式的话测试脚本匹配时视为错误**
  
  - You can modify `mr/worker.go`, `mr/coordinator.go`, and `mr/rpc.go`. You can temporarily modify other files for testing, but make sure your code works with the original versions; we'll test with the original versions. **只修改三个文件，其他文件改动只是为了debug，最终测试脚本还是使用原代码**
  
  - The worker should put intermediate Map output in files in the current directory, where your worker can later read them as input to Reduce tasks. **Map函数的输出在当前目录，使得待会可以将文件作为Reduce任务的输入**
  
  - `main/mrcoordinator.go` expects `mr/coordinator.go` to implement a `Done()` method that returns true when the MapReduce job is completely finished; at that point, `mrcoordinator.go` will exit. **当MapReuduce Job完成时Done()函数返回true，此时`mrcoordinator.go` 退出**
  
  - When the job is completely finished, the worker processes should exit. A simple way to implement this is to use the return value from `call()`: if the worker fails to contact the coordinator, it can assume that the coordinator has exited because the job is done, and so the worker can terminate too. Depending on your design, you might also find it helpful to have a "please exit" pseudo-task that the coordinator can give to workers.
  
    **当Job完成时所有worker应该都退出，一个简单的实现的方法是返回从call函数返回的值，如果worker联系coordinator失败可以假设coordinator已经退出，因为Job已经完成了，所以worker也可以退出，也可以自己实现coordinator主动告知worker退出的任务**



## 步骤

1. 其中的网络连接是通过**RPC**进行的，先读读文章给出的Go的RPC包的文章
2. coordinator做什么？
   - 划分任务
   - 等待worker连入（worker主动向coordinator请求任务），向它们分配任务
   - 周期性地检测worker是否正常运行(10 s)
   - 任务运行到接近90%发送备份
   - worker的任务完成之后告诉coordinator文件的存储的位置
3. worker的任务：
   - mapper：向coordinator请求任务，执行map函数，并将结果切分成R份，存入本地磁盘，告知coordinator
   - reducer：从coordinator获取文件位置（我认为只是获取机器标识以便使用RPC进行通信），执行reduce函数，





# Server.go

```
Package rpc provides access to the exported methods of an object across a
	network or other I/O connection.  A server registers an object, making it visible
	as a service with the name of the type of the object.  After registration, exported
	methods of the object will be accessible remotely.  A server may register multiple
	objects (services) of different types but it is an error to register multiple
	objects of the same type.
```

rpc包提供对通过网络或其他I/O连接访问一个对象的导出方法的能力。服务器注册一个对象，使其作为一个具有对象类型名称的服务。注册后，可以远程访问导出的对象方法。服务器可以注册不同类型的多个对象(服务)，但注册相同类型的多个对象是错误的。

**具有以下条件的方法才具有被远程访问的能力：**`func (t *T) MethodName(argType T1, replyType *T2) error`

```
- the method's type is exported.
- the method is exported.
- the method has two arguments, both exported (or builtin) types.
- the method's second argument is a pointer.
- the method has return type error.
```

Server端实现上面的函数可供其他客户端远程调用，然后Register注册服务，然后Listen监听等待请求

```
The server may handle requests on a single connection by calling ServeConn.  More
	typically it will create a network listener and call Accept or, for an HTTP
	listener, HandleHTTP and http.Serve.
-----------例如下面使用accept接受请求，然后rpc.ServerConn接受连接-------------
	for {
        conn, err := listener.Accept()
        if err != nil {
            log.Fatal("Accept error:", err)
        }
        rpc.ServeConn(conn)
    }
```

Client端向客户端的监听端口Dial拨号，得到一个该Client的结构体，通过该结构体访问服务，得到结果。

而该结构体提供远程服务的访问有同步Call和异步Go两种方式

```
The Call method waits for the remote call to complete 
while the Go method launches the call asynchronously and signals completion using the Call structure's Done channel.

// Synchronous call
		args := &server.Args{7,8}
		var reply int
		err = client.Call("Arith.Multiply", args, &reply)
		if err != nil {
			log.Fatal("arith error:", err)
		}
		fmt.Printf("Arith: %d*%d=%d", args.A, args.B, reply)

	or
		// Asynchronous call
		quotient := new(Quotient)
		divCall := client.Go("Arith.Divide", args, quotient, nil)
		replyCall := <-divCall.Done	// will be equal to divCall
		// check errors, print, etc.
```
