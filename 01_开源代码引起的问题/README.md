# 开源代码引起的问题

弱密码字典失效问题

首先，生成弱密码字典：

```shell
$ mkdir test
$ create-cracklib-dict -o test/new_dict words
```



然后，编译main

尝试吧

```shell
$ gcc main.c -o checkWeakPassword
$ ./checkWeakPassWord "Test@123" ./test/new_dict
```
