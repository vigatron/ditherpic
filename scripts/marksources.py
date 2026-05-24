import os

# путь к корню проекта
script_dir = os.path.dirname(os.path.abspath(__file__))
root_dir   = os.path.dirname(script_dir)
fnamehdr   = os.path.join(script_dir, "header.txt")

print("Script dir :", script_dir)
print("Root dir   :", root_dir)
print("cwd        :", os.getcwd())

# читаем заголовок
with open(fnamehdr, "r", encoding="utf-8") as f:
    header = f.read()

# расширения для обработки
extensions = (".h", ".hpp", ".cpp")

for subdir, _, files in os.walk(root_dir):
    for file in files:
        if file.endswith(extensions):
            print(file)
            filepath = os.path.join(subdir, file)
            with open(filepath, "r+", encoding="utf-8") as f:
                content = f.read()
                # проверка: если заголовок уже есть, пропускаем
                if header.strip() in content:
                    continue
                f.seek(0)
                f.write(header + "\n" + content)
