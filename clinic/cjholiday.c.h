/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(cjholiday_holiday_name__doc__,
"holiday_name($module, /, year=None, month=None, day=None, date=None)\n"
"--\n"
"\n"
"get name of holiday from ymd or datetime.date.");

#define CJHOLIDAY_HOLIDAY_NAME_METHODDEF    \
    {"holiday_name", (PyCFunction)cjholiday_holiday_name, METH_FASTCALL, cjholiday_holiday_name__doc__},

static PyObject *
cjholiday_holiday_name_impl(PyObject *module, PyObject *pyyear,
                            PyObject *pymonth, PyObject *pyday,
                            PyObject *date);

static PyObject *
cjholiday_holiday_name(PyObject *module, PyObject **args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"year", "month", "day", "date", NULL};
    static _PyArg_Parser _parser = {"|OOOO:holiday_name", _keywords, 0};
    PyObject *pyyear = NULL;
    PyObject *pymonth = NULL;
    PyObject *pyday = NULL;
    PyObject *date = NULL;

    if (!_PyArg_ParseStack(args, nargs, kwnames, &_parser,
        &pyyear, &pymonth, &pyday, &date)) {
        goto exit;
    }
    return_value = cjholiday_holiday_name_impl(module, pyyear, pymonth, pyday, date);

exit:
    return return_value;
}
/*[clinic end generated code: output=46399c1346e8276a input=a9049054013a1b77]*/
