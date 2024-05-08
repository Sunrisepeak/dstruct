import os
import re

def replace_strings_in_file(file_path):
    # 打开文件并读取内容
    with open(file_path, 'r', encoding='utf-8') as file:
        content = file.read()
    
    # 使用正则表达式替换所有匹配的字符串
    # 这里 '_m' 后跟任意字符，我们将这些字符捕获，然后在替换中使用
    # 假设 ` _mAbc` -> `mAbc_d`
    new_content = re.sub(r'([^a-zA-Z])_m(\w+)', r'\1m\2_d', content)

    # 将新内容写回文件
    with open(file_path, 'w', encoding='utf-8') as file:
        file.write(new_content)

def process_directory(directory):
    # 遍历目录中的所有文件
    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)

        # 检查是否为文件
        if os.path.isfile(file_path):
            # 替换文件中的字符串
            replace_strings_in_file(file_path)
            print(f"Processed {file_path}")
        else:
            process_directory(file_path)

# 指定要处理的目录
directory_path = os.getcwd() + '/core'
process_directory(directory_path)