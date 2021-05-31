多进程批处理,类似daemon,监听异常退出并重新拉起子进程

编译:
```bash
g++ -std=c++11 example.cpp multi_process_handler.cpp proc_utils.cpp -o example
```
