# Image Steganography Project

Welcome to the **Image Steganography** project! This tool allows you to hide secret messages inside image files. It's a simple yet powerful way to securely communicate hidden text through images. The technique used here is **Least Significant Bit (LSB) Encoding**, which modifies the pixels of an image to store your message without affecting the visual appearance.

## 🚀 Features

- Hide a text message within an image.
- Extract hidden messages from images.
- Fast and easy-to-use command-line tool.

## 📥 Installation

To get started, you'll need to clone this repository and compile the code. Follow the steps below to set up the project:

### 1. Clone the Repository
Use Git to clone the repository to your local machine:
```bash
git clone https://github.com/afsal2112/Image-stegnography.git
2. Navigate to the Project Directory
Once you have cloned the repository, move to the project folder:cd Image-stegnography

3. Compile the Code
To build the program, you'll need to compile the source code using GCC (GNU Compiler Collection):gcc *.c -o steg
After compiling, you will have an executable file named steg that you can run.

🛠️ How to Use
The program works in two main modes: Encoding (to hide a message) and Decoding (to extract the message). Below are the instructions for both.

Encoding: Hide a Message in an Image
To hide a secret message inside an image, use the encode (-e) mode. You'll need to provide the following:

* The path to the image where the message will be hidden.
* The secret message you'd like to encode.
* The name of the nw image file where the hidden message will be stored.

Example Command:
./steg -e beautiful.bmp secret.txt output_image.bmp

beautiful.bmp: The original image where the message will be hidden.
secret.txt: The file in there you want to enter the message you want to hide.
output_image.bmp: The new image that will contain the hidden message.

Decoding: Extract the Hidden Message
To retrieve the hidden message from an image, use the decode (-d) mode. You'll just need to provide the image with the hidden message.

Example command:
./steg -d output_image.bmp

This will output the hidden message stored in the image into the secret.txt file.


🤝 Contributing
If you'd like to improve this project, feel free to fork it and submit a pull request. Whether it's fixing bugs, adding new features, or improving documentation, contributions are always welcome.
