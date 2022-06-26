/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(cjholiday_holiday_name__doc__,
"holiday_name($module, /, year=<unrepresentable>,\n"
"             month=<unrepresentable>, day=<unrepresentable>,\n"
"             date=<unrepresentable>)\n"
"--\n"
"\n"
"get name of holiday from ymd or datetime.date.");

#define CJHOLIDAY_HOLIDAY_NAME_METHODDEF    \
    {"holiday_name", _PyCFunction_CAST(cjholiday_holiday_name), METH_FASTCALL|METH_KEYWORDS, cjholiday_holiday_name__doc__},

static PyObject *
cjholiday_holiday_name_impl(PyObject *module, PyObject *pyyear,
                            PyObject *pymonth, PyObject *pyday,
                            PyObject *date);

static PyObject *
cjholiday_holiday_name(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"year", "month", "day", "date", NULL};
    static _PyArg_Parser _parser = {NULL, _keywords, "holiday_name", 0};
    PyObject *argsbuf[4];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 0;
    PyObject *pyyear = NULL;
    PyObject *pymonth = NULL;
    PyObject *pyday = NULL;
    PyObject *date = NULL;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 0, 4, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (args[0]) {
        pyyear = args[0];
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (args[1]) {
        pymonth = args[1];
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    if (args[2]) {
        pyday = args[2];
        if (!--noptargs) {
            goto skip_optional_pos;
        }
    }
    date = args[3];
skip_optional_pos:
    return_value = cjholiday_holiday_name_impl(module, pyyear, pymonth, pyday, date);

exit:
    return return_value;
}
/*[clinic end generated code: output=76a7c6d42cfc64fd input=a9049054013a1b77]*/
