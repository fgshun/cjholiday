# 祝日判定

[AddinBox](http://addinbox.sakura.ne.jp/holiday_logic.htm) のコードを Python C 拡張として書き直したものです。

Python 3.6.5, 3.7.0 でのビルド、動作確認ができています。

```
>>> import cjholiday

>>> cjholiday.holiday_name(2020, 7, 24)
'スポーツの日'

>>> import datetime

>>> cjholiday.holiday_name(date=datetime.date(2020, 7, 24))
'スポーツの日'
```

## Building
```
pip install git+https://github.com/fgshun/cjholiday#egg=cjholiday
```

### for Windows 10
Python 3.6 もしくは 3.7 と Visual Studio 2017 を用意してください。
また、  pip install の実行の際に環境変数 `CL` に `/utf-8` を設定しておいてください。
つまり、 PowerShell では
```
set-item env:CL -value /utf-8
```
です。 cmd.exe では
```
SET CL=/utf-8
```
です。
