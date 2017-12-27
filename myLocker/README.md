### introduction
The program is designed to encrypt file and decrypt ciphertext based on a file which is invisible.  
The important file named `.key` which is in folder, it is a binary file containing information about some prime numbers.  
Our program works based on the prime numbers, it encrypt every character by XOR coding with single prime number in `.key` file, and decrypt process is same.  
Where do the prime numbers come from?  
The program will create a random number and resolute it into different prime factors when it receive command "encrypt" from user, the prime numbers will be write into `./folder/.key` file.  
When it need to decrypt, program read binary infomation from .key and translate it to prime numbers.   
    
The project is not finished, I will update it in the coming days.  

### contact
email: wei_yang1994@163.com  
blog:  http://blog.csdn.net/theArcticOcean  
