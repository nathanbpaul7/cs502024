// Retrieve todo from local storage or initialize an empty array
let todo = JSON.parse(localStorage.getItem("todo")) || [];
const todoInput = document.getElementById("todoInput");
const todoList = document.getElementById("todoList");
const todoCount = document.getElementById("todoCount");
const addButton = document.querySelector(".add-btn");
const deleteButton = document.getElementById("deleteButton");

// initialize
document.addEventListener("DOMContentLoaded", () => {
    addButton.addEventListener("click", AddTask);
    todoInput.addEventListener("keydown", (event) => {
        if (event.key === "Enter") {
          event.preventDefault(); // Prevents default Enter key behavior
          AddTask();
        }
      });
    deleteButton.addEventListener("click", deleteAllTasks);
    displayTasks();
});

// add task function
function AddTask() {
    const newTask = todoInput.value.trim();
    if (newTask !== "") {
        todo.push({ text: newTask, disabled: false });
        saveToLocalStorage();
        todoInput.value = "";
        displayTasks();
    }
}

// display task
function displayTasks() {
    todoList.innerHTML = ""; // sets the initial todo list to empty/no text before iterating over array: todo
    todo.forEach((item, index) => { // iterates over each item of todo, applying a function to item)
      const todoLine = document.createElement("p"); // goes into the DOM and creates a paragraph element, for each new todo
      todoLine.innerHTML = `
        <div class="todo-container">
          <input type="checkbox" class="todo-checkbox" id="input-${index}" ${ // note that js variable, index, to name our id's as they iterate, ensuring they're referencable later
        item.disabled ? "checked" : ""
      }>
          <p id="todo-${index}" class="${
        item.disabled ? "disabled" : ""
      }" onclick="editTask(${index})">${item.text}</p>
        </div>
      `;
      todoLine.querySelector(".todo-checkbox").addEventListener("change", () =>
        toggleTask(index)
      );
      todoList.appendChild(todoLine);
    });
    todoCount.textContent = todo.length;
  }

  function editTask(index) {
    const todoItem = document.getElementById(`todo-${index}`);
    const existingText = todo[index].text;
    const inputElement = document.createElement("input");

    inputElement.value = existingText;
    todoItem.replaceWith(inputElement);
    inputElement.focus();

    inputElement.addEventListener("blur", function () {
      const updatedText = inputElement.value.trim();
      if (updatedText) {
        todo[index].text = updatedText;
        saveToLocalStorage();
      }
      displayTasks();
    });
  }

  function toggleTask(index) {
    todo[index].disabled = !todo[index].disabled;
    saveToLocalStorage();
    displayTasks();
  }

  function deleteAllTasks() {
    todo = [];
    saveToLocalStorage();
    displayTasks();
  }

  function saveToLocalStorage() {
    localStorage.setItem("todo", JSON.stringify(todo));
  }
