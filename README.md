# 日本の祝日判定
[AddinBox](http://addinbox.sakura.ne.jp/holiday_logic.htm) のコードを Python C 拡張として書き直したものです。
Python 3.7 以降でのビルド、動作確認ができています。

```
>>> import cjholiday

>>> cjholiday.holiday_name(2021, 7, 23)
'スポーツの日'

>>> import datetime

>>> cjholiday.holiday_name(date=datetime.date(2021, 7, 23))
'スポーツの日'
```

## Install
```
pip install git+https://github.com/fgshun/cjholiday.git@v1.4.3#egg=cjholiday
```

### Install on Windows
```
# Python 3.11
pip install https://github.com/fgshun/cjholiday/releases/download/v1.4.3/cjholiday-1.4.3-cp311-cp311-win_amd64.whl
# Python 3.10
pip install https://github.com/fgshun/cjholiday/releases/download/v1.4.3/cjholiday-1.4.3-cp310-cp310-win_amd64.whl
# Python 3.9
pip install https://github.com/fgshun/cjholiday/releases/download/v1.4.3/cjholiday-1.4.3-cp39-cp39-win_amd64.whl
# Python 3.8
pip install https://github.com/fgshun/cjholiday/releases/download/v1.4.3/cjholiday-1.4.3-cp38-cp38-win_amd64.whl
# Python 3.7
pip install https://github.com/fgshun/cjholiday/releases/download/v1.4.3/cjholiday-1.4.3-cp37-cp37m-win_amd64.whl
```

### Building on Windows
Python 3.7, 3.8, 3.9 3.10 もしくは 3.11 と Visual Studio 2017 を用意してください。
また、 環境変数 `CL` に `/utf-8` を設定しておいてください。

## C API
Python C 拡張からの直接の使用ができます。
そのためには `cjholiday.h` を include してモジュール初期化時に `CJHoliday_IMPORT` マクロを起動しておいてください。

    PyMODINIT_FUNC
    PyInit_usecjholiday(void)
    {
        if (!(CJHoliday_IMPORT)) {
            return NULL;
        }
        return PyModuleDef_Init(&usecjholiday_module);
    }

`static PyObject *CJHoliday_HolidayName(int year, int month, int day)`  
年月日を表す 3 つの int からその日が祝日かどうかを判別します。祝日であれば祝日名 str を返します。異なれば None を返します。

`static PyObject *CJHoliday_HolidayNameDate(PyObject *date)`  
datetime.date からその日が祝日かどうかを判別します。祝日であれば祝日名 str を返します。異なれば None を返します。
