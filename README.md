## Introduction
Serving TexLive Files in an On-Demand Manner. You do not necessary have to host it by yourself and we provide one public server in https://www.swiftlatex.com/dl/tex/<FileYouWant>

This server generates vf fonts in real time.
It requires two changes in the existing toolchain.
First dvipdfmx should silently ignore the invalid char in tfm.c
Second dviasm.py should generate 0-255 set1 commands when set1 command is encountered.

 