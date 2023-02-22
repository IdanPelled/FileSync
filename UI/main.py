import configparser
import tkinter as tk
from tkinter import filedialog


PATH = "FileSync/UI/config.ini"
SECTION = "app"
FOLDER_PATH = "folder"


def read_configuration_file(path: str) -> configparser.ConfigParser:
    config = configparser.ConfigParser()
    config.read(path)
    return config


def read_val(config: configparser.ConfigParser, name: str) -> str:
    return config.get(SECTION, name)


def update_val(config: configparser.ConfigParser, name: str, val: str) -> None:
    config.set(SECTION, name, val)


def update_folder_path(val: str, config: configparser.ConfigParser):
    return update_val(config, FOLDER_PATH, val)


def get_folder_path(config: configparser.ConfigParser) -> str:
    return read_val(config, FOLDER_PATH)


def save_config_file(path: str, config: configparser.ConfigParser):
    with open(path, 'w') as configfile:
        config.write(configfile)




def main():

    def get_folder():
        path.config(text=filedialog.askdirectory())

    def save_path():
        update_folder_path(path.cget('text'), config)
        save_config_file(PATH, config)


    root = tk.Tk()
    root.geometry("500x300")  # set the size of the window
    root.config(bg='#f0f0f0')  # set the background color of the window
    root.title("FileSync")  # set the title of the window
    config = read_configuration_file(PATH)


    header = tk.Label(root, text='Synchronized Folder:', font=("Arial", 14), fg='#333333',bg='#f0f0f0')
    header.grid(row=0, column=0, padx=20, pady=20, sticky='W')

    path = tk.Label(root, text=get_folder_path(config), font=("Arial", 12), fg='#333333',bg='#f0f0f0')
    path.grid(row=1, column=0, padx=20, pady=20, columnspan=2, sticky='W')

    select_btn = tk.Button(root, text='Select folder', width=15, font=("Arial", 12), command=get_folder)
    select_btn.grid(row=2, column=0, padx=20, pady=20, sticky='W')

    save_btn = tk.Button(root, text='Save', width=15, font=("Arial", 12), command=save_path)
    save_btn.grid(row=2, column=1, padx=20, pady=20, sticky='E')

    root.mainloop()



if __name__ == "__main__":
    main()