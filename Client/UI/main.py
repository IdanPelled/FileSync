import tkinter as tk
import configparser
from tkinter import filedialog

from interface import _login, _signup


CONFIG_PATH = "C:\ProgramData\FileSync\Config\config.ini"
SECTION = "app"
FOLDER_PATH = "folder"

# Define colors
RED = "#ed3a59"
BLACK = "#001524"
BLUE = "#15616d"
LIGHT_BLUE = "#afc9cd"
WHITE = "#ffffff"

class FileSyncUI:
    pages = {}

    def __init__(self):
        self.init_config()
        self.run()

    def run(self):
        if self.is_authenticated():
            self.navigate_to_page("homepage")
        
        else:
            self.navigate_to_page("login")
        
        self.root.mainloop()

    def is_authenticated(self):
        return (
            self.config.get(SECTION, "username")
            and self.config.get(SECTION, "password")
            and self.config.get(SECTION, "token")
        )

    def init_config(self):
        self.root = tk.Tk()
        self.config = self.read_configuration_file(CONFIG_PATH)

        self.root.geometry("800x600")
        self.root.configure(bg=WHITE)
        self.root.resizable(False, False)
        self.root.title("FileSync")
        
        self.icon_image = tk.PhotoImage(file="Client/UI/images/logo.png")
        self.logo_image = tk.PhotoImage(file="Client/UI/images/small full logo white.png")
        self.root.tk.call('wm', 'iconphoto', self.root._w, self.icon_image)

        self.create_footer()
        
        self.create_login_page()
        self.create_signup_page()
        self.create_homepage()

    def create_header(self, frame, is_auth):
        header_frame = tk.Frame(frame, bg=BLACK)
        header_frame.pack(side="top", fill="x")

        self.logo_label = tk.Label(header_frame, image=self.logo_image, bg=BLACK)
        self.logo_label.pack(side="left")

        if is_auth:

            logout_label = tk.Label(header_frame, text="Logout", font=("Arial", 14), fg=WHITE, bg=BLACK)
            logout_label.bind("<Button-1>", lambda _: self.logout())
            logout_label.pack(side="right", padx=2, pady=10)

            welcome_label = tk.Label(header_frame, text=f"Welcome {self.get_username()}   | ", font=("Arial", 14), fg=WHITE, bg=BLACK)
            welcome_label.pack(side="right", padx=2, pady=10)
            
        else:

            signup_label = tk.Label(header_frame, text="SignUp", font=("Arial", 14), fg=WHITE, bg=BLACK)
            signup_label.bind("<Button-1>", lambda _: self.navigate_to_page("signup"))
            signup_label.pack(side="right", padx=2, pady=10)

            login_label = tk.Label(header_frame, text="Login   | ", font=("Arial", 14), fg=WHITE, bg=BLACK)
            login_label.bind("<Button-1>", lambda _: self.navigate_to_page("login"))
            login_label.pack(side="right", padx=2, pady=10)

    def create_footer(self):
        self.footer_frame = tk.Frame(self.root, bg=BLUE)
        self.footer_frame.pack(side="bottom", fill="x")

        self.footer_text = tk.Label(self.footer_frame, text="pelled.idan@gmail.com  |  Git repository  |  © 2023 FileSync All Rights Reserved.", fg=WHITE, bg=BLUE)
        self.footer_text.pack(side="top", padx=10, pady=5)

    def create_labe(self, frame, text, fg):
        label = tk.Label(frame, text=text, bg=WHITE, fg=fg)
        label.pack(side="top", padx=10, pady=5, anchor="w")
        return label
    
    def create_main_header(self, frame, text):
        header = tk.Label(frame, text=text, fg=RED, bg=WHITE, font=("Arial", 22, "bold"))
        header.pack(side="top", padx=10, pady=20, anchor="w")
        return header

    def create_text_box(self, frame, var ,**kw):
        text_box = tk.Entry(frame, background=LIGHT_BLUE, textvariable=var, **kw)
        text_box.pack(side="top", padx=10, pady=5, fill="x")
        return text_box

    def create_button(self, frame, text, bg, fg, action):
        button = tk.Button(
            frame,
            text=text,
            bg=bg,
            fg=fg,
            font=("Arial", 12, "bold"),
            highlightbackground=WHITE,
            command=action
        )
        button.pack(side="left", padx=10, pady=20)
        return button
    
    def create_login_page(self):
        self.login_frame = tk.Frame(self.root, bg=WHITE)
        self.create_header(self.login_frame, False)
        self.login_header = self.create_main_header(self.login_frame, "Login")

        self.username_label = self.create_labe(self.login_frame, "Username:", BLACK)
        self.login_form_username = tk.StringVar()
        self.username_entry = self.create_text_box(self.login_frame, self.login_form_username)

        self.password_label = self.create_labe(self.login_frame, "Password:", BLACK)
        self.login_form_password = tk.StringVar()
        self.password_entry = self.create_text_box(self.login_frame, self.login_form_password, show="*")

        self.login_error_label = tk.Label(self.login_frame, text="", fg=RED, bg=WHITE)
        self.login_error_label.pack(side="top", padx=10, pady=5, anchor="w")

        self.login_button = self.create_button(self.login_frame, "Login", RED, BLACK, self.login)

        self.login_frame.pack(side="top", fill="both", expand=True)
        self.pages.update({"login": self.login_frame})

    def create_signup_page(self):
        self.signup_frame = tk.Frame(self.root, bg=WHITE)
        self.create_header(self.signup_frame, False)
        self.signup_header = self.create_main_header(self.signup_frame, "Sign Up")
        
        self.username_label = self.create_labe(self.signup_frame, "Username:", BLACK)
        self.signup_form_username = tk.StringVar()
        self.username_entry = self.create_text_box(self.signup_frame, self.signup_form_username)

        self.password_label = self.create_labe(self.signup_frame, "Password:", BLACK)
        self.signup_form_password = tk.StringVar()
        self.password_entry = self.create_text_box(self.signup_frame, self.signup_form_password, show="*")

        self.confirm_password_label = self.create_labe(self.signup_frame, "Confirm password:", BLACK)
        self.signup_form_confirm_password = tk.StringVar()
        self.confirm_password_entry = self.create_text_box(self.signup_frame, self.signup_form_confirm_password, show="*")

        self.signup_error_label = tk.Label(self.signup_frame, text="", fg=RED, bg=WHITE)
        self.signup_error_label.pack(side="top", padx=10, pady=5, anchor="w")


        self.signup_button = self.create_button(self.signup_frame, "Sign Up", RED, BLACK, self.signup)

        self.signup_frame.pack(side="top", fill="both", expand=True)
        self.pages.update({"signup": self.signup_frame})

    def create_homepage(self):
        self.homepage_frame = tk.Frame(self.root, bg=WHITE)
        self.create_header(self.homepage_frame, True)
        self.homepage_header = self.create_main_header(self.homepage_frame, "Syncronised Folder")

        self.folder_label = self.create_labe(self.homepage_frame, "Folder Path:", RED)
        self.folder_entry = self.create_labe(self.homepage_frame, self.get_folder_path(), BLACK)


        self.change_folder_button = self.create_button(self.homepage_frame, "Change Folder", RED, BLACK, self.change_folder)
        self.save_folder_button = self.create_button(self.homepage_frame, "Save Folder", RED, BLACK, self.save_folder)
        
        self.homepage_frame.pack(side="top", fill="both", expand=True)
        self.pages.update({"homepage": self.homepage_frame})

    def navigate_to_page(self, page_name):
        self.hide_all()

        self.current_page = page_name
        page = self.pages[page_name]
        page.update()
        page.pack(side="top", fill="both", expand=True)

    def hide_all(self):
        for _, p in self.pages.items():
            p.pack_forget()
    
    def login(self):
        username = self.login_form_username.get()
        password = self.login_form_password.get()

        if not (username and password):
            self.login_error_label.config(text="Username and password are required")
            return

        if _login(username, password):
            self.navigate_to_page("homepage")

        else:
            self.login_error_label.config(text="Incorrect username or password")

    def signup(self):
        print("Signing up...")
        username = self.signup_form_username.get()
        password = self.signup_form_password.get()
        confirm_password = self.signup_form_confirm_password.get()

        if not (username and password and confirm_password):
            self.signup_error_label.config(text="All fields are required")
            return

        if password != confirm_password:
            self.signup_error_label.config(text="Passwords do not match")
            return


        if _signup(username, password):
            self.navigate_to_page("login")

        else:
            self.signup_error_label.config(text="Signup failed")

    def logout(self):
        for s in (FOLDER_PATH, "token", "username", "password"):
            self.config.set(SECTION, s, "")
        self.save_configuration_file(CONFIG_PATH)
            
        self.navigate_to_page("login")
        print("Loggin out...")

    def change_folder(self):
        # Placeholder change folder function
        print("Changing folder...")
        self.folder_entry.config(text=filedialog.askdirectory())

    def save_folder(self):
        # Placeholder save folder function
        print("Saving folder...")
        self.save(self.folder_entry.cget('text'))

    def save_configuration_file(self, path):
        with open(path, "w") as fh:
            self.config.write(fh)

    def read_configuration_file(self, path):
        config = configparser.ConfigParser()
        config.read(path)
        return config

    def get_folder_path(self):
        self.config = self.read_configuration_file(CONFIG_PATH)
        return self.config.get(SECTION, FOLDER_PATH)

    def update_folder_path(self, val):
        self.config = self.read_configuration_file(CONFIG_PATH)
        self.config.set(SECTION, FOLDER_PATH, val)
    
    def get_username(self):
        self.config = self.read_configuration_file(CONFIG_PATH)
        return self.config.get(SECTION, "username")

    def save_config_file(self, path: str):
        with open(path, 'w') as configfile:
            self.config.write(configfile)

    def save(self, path):
        self.update_folder_path(path)
        self.save_config_file(CONFIG_PATH)
    
    def clear_form(self):
        page = self.pages[self.current_page]
        for widget in page.winfo_children():
            if isinstance(widget, tk.Entry) and widget["textvariable"] is not None:
                var = widget["textvariable"]
                if isinstance(var, tk.StringVar):
                    var.set("")


if __name__ == "__main__":
    # Create and run the UI
    FileSyncUI()