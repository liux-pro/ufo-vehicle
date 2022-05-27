# 参考https://www.jianshu.com/p/5ce20e38696c
# 原来的文件编码都是gb18030，很难受，用脚本转成UTF-8
import codecs
import os

# 文件所在目录
def utf_ansi(ar):
    file_path = r'{0}'.format(ar)
    files = os.listdir(file_path)

    for file in files:
        file_name = file_path + '\\' + file
        f = codecs.open(file_name, 'r', 'gb18030')
        ff = f.read()
        file_object = codecs.open(file_path + '\\' + file, 'w', 'utf-8')
        file_object.write(ff)
    return 'ok'
utf_ansi("./Libraries/inc")
utf_ansi("./Libraries/src")
print('ok')
