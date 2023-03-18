本程序支持使用命令行参数。

```bash
$ learning_focus --help
Usage: learning_focus [--help] [--correction CORRECTION] [--device DEVICE] [--debug] [--delay DELAY] [--noconsole] [--tolerance TOLERANCE]

Learning Focus command line arguments.

Optional arguments:
  --help                  Show this help message and exit.
  --correction CORRECTION Correction threshold (Between 0 and 1) of error face measurement. (Default: 0.05)
  --device DEVICE         Device ID of the camera using. (Default: 0)
  --debug                 Enable debug mode. (Default: Disable)
  --delay DELAY           Delay time (Millisecond) after predicting. (Default: -1)
  --noconsole             Hide the console window. (Default: Disable)
  --tolerance TOLERANCE   Tolerance (Between 0 and 1) for face comparing. (Default: 0.5)
```

`--help` 为显示帮助信息。

`--correction` 为自动回收误测人脸数据比例阈值，默认为 0.05。

`--device` 为摄像头设备的编号，默认为 0。

`--debug` 为是否启用调试模式，默认禁用。

`--delay` 为统一延时时间（毫秒），默认 -1 （不使用）。

`--noconsole` 为隐藏控制台窗口（仅限 Windows 平台），默认禁用。

`--tolerance` 为人脸比对的相似度阈值，默认 0.5。