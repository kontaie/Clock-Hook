# Windows UI Automation Event Listener

This project demonstrates how to use **Windows UI Automation** to detect and respond to user interface events.  
It is built on top of the [IUIAutomation COM interface](https://learn.microsoft.com/en-us/windows/win32/api/uiautomationclient/nn-uiautomationclient-iuiautomation), which is part of the **Windows UI Automation framework**.

## 📌 What it does
- Tracks the element under your mouse cursor.
- When you hover over the **Start button** (or any element you choose), it registers an event handler.
- The event handler listens for **Invoke events** (for example, when the element is clicked or activated).
- When triggered, it prints out the element’s name to the console.

In short, this project shows how your code can "listen" to UI actions happening in Windows and respond to them.
For now this demo just invokes the event handler when the Start button on windows clock timer is pressed.

## 🛠️ Technologies Used
- **C++** with COM
- **Windows UI Automation Core Library**

## 🚀 Getting Started
1. Clone the repository.
2. Open the project in **Visual Studio**.
3. Build and run it on **Windows**.
4. Move your cursor over UI elements—when it detects the "Start" button, it registers an event listener.
5. Try clicking the button, and watch the console output.

## 📚 Learn More
If you’re new to Windows UI Automation, check out Microsoft’s official documentation:  
🔗 [Windows UI Automation Overview](https://learn.microsoft.com/en-us/windows/win32/winauto/entry-uiauto-win32)  
🔗 [IUIAutomation Interface](https://learn.microsoft.com/en-us/windows/win32/api/uiautomationclient/nn-uiautomationclient-iuiautomation)

---

