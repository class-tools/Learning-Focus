本程序支持使用命令行参数。

**若要终止检测，请选中 Face 窗口并按下 ESC，切勿直接关闭控制台窗口。**

```bash
$ ./LF --help
Usage: LF [-h] [--camera VAR] [--datalist] [--datashow VAR] [--extra] [--windowwidth VAR] [--windowheight VAR]

Optional arguments:
  -h, --help            shows help message and exits
  -v, --version         prints version information and exits
  -c, --camera          specify the camera id of the video capturing device to use [default: 0]
  -dl, --datalist       show data list
  -ds, --datashow       visualize data with the key specified [default: ""]
  -e, --extra           show extra information
  -ww, --windowwidth    specify the width of the window [default: 640]
  -wh, --windowheight   specify the height of the window [default: 480]
```
