# 祝日判定

[AddinBox](http://addinbox.sakura.ne.jp/holiday_logic.htm) のコードを Python C 拡張として書き直したものです。

Python 3.6.6, 3.7.0 でのビルド、動作確認ができています。

```
>>> import cjholiday

>>> cjholiday.holiday_name(2020, 7, 24)
'スポーツの日'

>>> import datetime

>>> cjholiday.holiday_name(date=datetime.date(2020, 7, 24))
'スポーツの日'
```

## Install
```
pip install git+https://github.com/fgshun/cjholiday#egg=cjholiday
```

### for Windows
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


## C API
Python C 拡張からの直接の使用ができます。
そのためには `cjholiday.h` を include してモジュール初期化時に `CJHoliday_IMPORT` マクロを起動しておいてください。

`static PyObject *CJHoliday_HolidayName(int year, int month, int day)`  
年月日を表す 3 つの int からその日が祝日かどうかを判別します。祝日名もしくは None を返します。

`static PyObject *CJHoliday_HolidayNameDate(PyObject *date)`  
datetime.date からその日が祝日かどうかを判別します。祝日名もしくは None を返します。
