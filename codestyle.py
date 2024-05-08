import os
import re

rules = {
    r'__([^a-zA-Z])' : r'_e\1',
    r'([^a-zA-Z])_m(\w+)': r'\1m\2_d', # 'X_mAbc' -> 'XmAbc_d'
    r'([^a-zA-Z])_([A-Z])(\w+)': r'\1\2\3_' # 'X_Abc' -> 'XAbc_'
}

def replace_strings_in_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        content = file.read()
    
    for k, v in rules.items():
        content = re.sub(k, v, content)

    with open(file_path, 'w', encoding='utf-8') as file:
        file.write(content)

def process_directory(directory):
    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)

        if os.path.isfile(file_path):
            replace_strings_in_file(file_path)
            print(f"Processed {file_path}")
        else:
            process_directory(file_path)

# 指定要处理的目录
directory_path = 'memory'
process_directory(directory_path)