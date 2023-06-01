 
 #pragma once
 #include <cstdio>
 #include <stdexcept>
 #include "bloc.hpp"

 namespace util{

   inline bloc<u_char> readfile(std::string path){
     FILE* file = fopen(path.c_str(),"r");
     if(file==NULL){
       throw std::runtime_error("Cannot open file: "+path);
     }
     fseek(file,0,SEEK_END);
     size_t len=ftell(file);
     rewind(file);

     bloc ret(len);
     size_t count=fread(ret,1,len,file);
     fclose(file);
     if(count!=len){
       ret.destroy();
       throw std::runtime_error("Error reading file: "+path);
     }
     return ret;
   }

   inline void writefile(std::string path, bloc<u_char> data){
     FILE* file = fopen(path.c_str(),"w");
     if(file==NULL){
       throw std::runtime_error("Cannot open file: "+path);
     }

     size_t count=fwrite(data,1,data.size,file);
     fclose(file);
     if(count!=data.size){
       throw std::runtime_error("Error writing file: "+path);
     }
   }

 }
