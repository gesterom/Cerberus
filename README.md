 # Cerberus

Welcome to Cerberus, a powerful and extendable programming language designed to facilitate inter-language communication. This repository contains the source code for the Cerberus compiler, supporting libraries, and example applications.
Features
 - Inter-Language Communication: Seamlessly interact with code and data across different programming languages.
 - Custom Language Components: Extend the core language by defining new grammar and functionalities.
 - DLL Loading: Load and use language components dynamically from DLLs.
 - Flexible Type System: Define custom types and procedures.
 - Global Symbol Table: Access and manipulate symbols globally during compilation.
 - LLVM IR Emission: Generate LLVM Intermediate Representation for efficient compilation.
 - Modern C++: Developed with C++20, ensuring robust and efficient code.

 ## Getting Started
Prerequisites
 - Visual Studio 2022
 - LLVM

 ## Building the Compiler
  Clone the repository:

```sh
git clone https://github.com/yourusername/Cerberus.git
cd Cerberus
```
Open Visual Studio 2022.

Open the Cerberus solution file (Cerberus.sln).

Build the solution by selecting Build > Build Solution (or press Ctrl+Shift+B).
 ## Running the Compiler

After building the project, you can use the compiler to compile code written in Cerberus.

```sh
./CerberusCompiler your_code_file.dog
```
 ## Example Code

Here's an example of code written in Cerberus:

```javascript
procedure main() -> Int
    printf("hello world");
    return 0;

type Pair
    Int x;
    Let y: Int32

procedure printPair(Pair :p) -> String
    return "\{{p.x}, {p.y}\}";
```
Here's an example demonstrating inter-language communication:

```sql
typedef UserId Int64
query get_users(UserId userId)
    select * from users where id = :userid
```
 ## Extending the Language

You can create custom language components and load them as DLLs. A language component defines new grammar and extends the core language functionality. Below is a conceptual overview of how to create and load a language component.

1. Define a new grammar and functionalities in your component.
2. Compile your component into a DLL.
3. Load the DLL in your code using the provided interfaces.

 ## Contributing

Contributions are welcome! Please fork the repository and submit pull requests. For major changes, please open an issue first to discuss what you would like to change.

1. Fork the repository
2. Create a new branch (git checkout -b feature-branch)
3. Commit your changes (git commit -am 'Add new feature')
4. Push to the branch (git push origin feature-branch)
5. Create a pull request

 ## License

This project is licensed under the MIT License. See the LICENSE file for details.
Contact
