 /*******************************************************
 * Name: Ruben Navarro
 * an assembler of a subset of the MIPS assembly language
 * Compile:    "gcc -g assem.c"
 ********************************************************/

#include <cstdio>
#include <iostream>
#include <cstring>
#include <sstream>
#include <bitset> 
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
#include <stdlib.h>

#define MAXLINE 80
#define MAXREG  5

char        locateReg  (char *lookfor);    // prototype for locate register
std::string hexResult  (std::string s);    // prototype for return hex result 

int main()
{
  /* VARIABLES */

  char line[MAXLINE]={0};                           
  char oper[MAXLINE];
  char rd[MAXREG],rs[MAXREG],rt[MAXREG];
  char imme[MAXREG],addr[MAXREG],line2[MAXLINE];
  char text[MAXREG];
  char nval[MAXREG];
  bool  data = false;
  int op = 0;
  int rd_ = 0;
  int rs_ = 0;
  int rt_ = 0;
  int shamt = 0;
  int funct = 0;
  int imm;
  int counter = 0;
  int deductext = 0;
  int n = 0; 
  int wordcount = 0;
  char lab[MAXREG];
  int track = 0;

  /* containers */
  std::map<std::string,int> mymap;  

  /*first pass to read input */ 
  while (fgets(line, MAXLINE, stdin))
  {
    /* scan for label for R format and insert into map */
    if (sscanf(line, "%s %s $%[^,],$%[^,],$%s", lab, oper, rd, rs, rt) == 5)
    {
      std::string holdstrr = lab; 
      holdstrr.resize(holdstrr.size() - 1); 
      mymap.insert( std::pair<std::string,int>(holdstrr,track));
    }
    /* scan for label for I format and store into map */
    else if ((sscanf(line, "%s %s $%[^,],%[^(]($%[^)]", lab, oper, rt, imme, rs) == 5) || (sscanf(line, "%s %s $%[^,],$%[^,],%s", lab, oper, rs, rt, imme) == 5))
    {
      std::string holdstri = lab;
      holdstri.resize(holdstri.size() - 1);
      mymap.insert(std::pair<std::string,int>(holdstri,track));
    }
    /* scan for label in .text lines and store into map */
    else if (sscanf(line, "%s .%s %s", lab, text, nval) == 3)  
     {
       std::string holdstrt = lab;
       holdstrt.resize(holdstrt.size() - 1);
       mymap.insert(std::pair<std::string,int>(holdstrt,track));
       /* counter for .word display */
        deductext += 1;
       if (strcmp(text,"word") == 0)
          wordcount += 1; 
      
       data = true;
       n = atoi(nval);    
     }
    if ((sscanf(line, " .%s", text)) == 1)
    {
      deductext += 1; 
      if (strcmp(text,"word") == 0)
        wordcount += 1;
    }
    /* keeps track of lines */
     ++track;
 }
  /* reset to start */ 
  rewind(stdin);

  /* print number of instructions on top of output */
  std::cout << (track - deductext) << " " << wordcount << std::endl;

  /* Second pass to read and execute specified instructions */
  while (fgets(line, MAXLINE, stdin)) {
   /* Reads line, if label is found copies line without label to new string */
   int i = 0;                                           
   bool flag = false;
   int x = 0;

   for (; i < strlen(line) - 1; ++i)
   {
     if (line[i] == ':')
     {
       flag = true;
       break;
     }
     else
     ++i;
   }
   /* if a label is found on line remove label and create a new line for proper
      formatting */
   if (flag)
       strncpy(line2, line + (i + 2), (strlen(line) - 1));
   else 
     strcpy(line2, line); 
     
     /* Scan R format instruction with 3 args - SUPPORTS: ADDU, SLT, SUBU, AND, OR  */
   if (sscanf(line2, "%s $%[^,],$%[^,],$%s", oper, rd, rs, rt) == 4) {

      if (strcmp(oper, "addu") == 0)              // ADDU
      {
        ++counter;
        /* locate registry and assign to int */
        rd_ = locateReg(rd);
        rs_ = locateReg(rs);
        rt_ = locateReg(rt);

        /* locally used variables */
        std::stringstream ss;
        std::string hexResults;
        std::string adduresult;

        /* convert decimals into binary */  
        std::bitset<6> opb (0);
        std::bitset<5> rdb (rd_);
        std::bitset<5> rsb (rs_);
        std::bitset<5> rtb (rt_);
        std::bitset<5> shamtb (0); 
        std::bitset<6> functb (33);
        
        /* assign bits to stringstream */
        ss << opb;
        ss << rsb;
        ss << rtb;
        ss << rdb;
        ss << shamtb;
        ss << functb;
          
        /* pass string to hexResult function for hex output */
        std::string s = ss.str();
        ss.clear();
        std::cout << hexResult(s) << std::endl;  
        s.clear();
      }
      else if (strcmp(oper, "slt") == 0)              // SLT
      {
        ++counter;
        /* locate registry and assign to int */
        rd_ = locateReg(rd);
        rs_ = locateReg(rs);
        rt_ = locateReg(rt);
      
        /* locally used variables */  
        std::stringstream ss;

       /* convert decimals into binary */
        std::bitset<6> opb (0);
        std::bitset<5> rdb (rd_);
        std::bitset<5> rsb (rs_);
        std::bitset<5> rtb (rt_);
        std::bitset<5> shamtb (0);
        std::bitset<6> functb (42);

        /* assign bits to stringstream */
        ss << opb;
        ss << rsb;
        ss << rtb;
        ss << rdb;
        ss << shamtb;
        ss << functb;

        /* pass string to hexResult function for hex output */
        std::string s = ss.str();
        ss.clear();
        std::cout << hexResult(s) << std::endl;
        s.clear();
      }
      else if (strcmp(oper, "and") == 0)             // AND
      {
        ++counter;
        /* locate registry and assign to int */
        rd_ = locateReg(rd);
        rs_ = locateReg(rs);
        rt_ = locateReg(rt);

        /* locally used variables */
        std::stringstream ss;

        /* convert decimals into binary */
        std::bitset<6> opb (0);
        std::bitset<5> rdb (rd_);
        std::bitset<5> rsb (rs_);
        std::bitset<5> rtb (rt_);
        std::bitset<5> shamtb (0);
        std::bitset<6> functb (36);

        /* assign bits to stringstream */
        ss << opb;
        ss << rsb;
        ss << rtb;
        ss << rdb;
        ss << shamtb;
        ss << functb;

        /* pass string to hexResult function for hex output */
        std::string s = ss.str();
        ss.clear();
        std::cout << hexResult(s) << std::endl;
        s.clear();
      }
      else if (strcmp(oper, "or") == 0)             // OR
      {
        ++counter;
        /* locate registry and assign to int */

        rd_ = locateReg(rd);
        rs_ = locateReg(rs);
        rt_ = locateReg(rt);

        /* locally used variables */
        std::stringstream ss;

        /* convert decimals into binary */
        std::bitset<6> opb (0);
        std::bitset<5> rdb (rd_);
        std::bitset<5> rsb (rs_);
        std::bitset<5> rtb (rt_);
        std::bitset<5> shamtb (0);
        std::bitset<6> functb (37);

        /* assign bits to stringstream */
        ss << opb;
        ss << rsb;
        ss << rtb;
        ss << rdb;
        ss << shamtb;
        ss << functb;

        /* pass string to hexResult function for hex output */     
        std::string s = ss.str();
        ss.clear();
        std::cout << hexResult(s) << std::endl;
        s.clear();
      }
      else if (strcmp(oper, "subu") == 0)              // SUBU
      {
        ++counter;
        /* locate registry and assign to int */

        rd_ = locateReg(rd);
        rs_ = locateReg(rs);
        rt_ = locateReg(rt);

        /* locally used variables */
        std::stringstream ss;

        /* convert decimals into binary */
        std::bitset<6> opb (0);
        std::bitset<5> rdb (rd_);
        std::bitset<5> rsb (rs_);
        std::bitset<5> rtb (rt_);
        std::bitset<5> shamtb (0);
        std::bitset<6> functb (35);

        /* assign bits to stringstream */
        ss << opb;
        ss << rsb;
        ss << rtb;
        ss << rdb;
        ss << shamtb;
        ss << functb;

        /* pass string to hexResult function for hex output */      
        std::string s = ss.str();
        ss.clear();
        std::cout << hexResult(s) << std::endl;
        s.clear();
      }
   }
   /* Scan for R instruction format with 2 args, SUPPORTS: MULT and DIV */
   else if (sscanf(line2, "%s $%[^,],$%[^\n]", oper, rs, rt) == 3){
    
     if (strcmp(oper, "mult") == 0)                  // MULT
      {
          ++counter;
          /* locate registry and assign to int */

          rs_ = locateReg(rs);
          rt_ = locateReg(rt);

          /* locally used variables */
          std::stringstream ss;

          /* convert decimals into binary */
          std::bitset<6> opb (0);
          std::bitset<5> rsb (rs_);
          std::bitset<5> rtb (rt_);
          std::bitset<10> shamtb (0);
          std::bitset<6> functb (24);

          /* assign bits to stringstream */
          ss << opb;
          ss << rsb;
          ss << rtb;
          ss << shamtb;
          ss << functb;

          /* pass string to hexResult function for hex output */
          std::string s = ss.str();
          ss.clear();
          std::cout << hexResult(s) << std::endl;
          s.clear();
       }
      else if (strcmp(oper, "div") == 0)               // DIV
        {
          rs_ = locateReg(rs);
          rt_ = locateReg(rt);

          /* locally used variables */
          std::stringstream ss;

          /* convert decimals into binary */
          std::bitset<6> opb (0);
          std::bitset<5> rsb (rs_);
          std::bitset<5> rtb (rt_);
          std::bitset<10> shamtb (0);
          std::bitset<6> functb (26);

          /* assign bits to stringstream */
          ss << opb;
          ss << rsb;
          ss << rtb;
          ss << shamtb;
          ss << functb;

          /* pass string to hexResult function for hex output */
          std::string s = ss.str();
          ss.clear();
          std::cout << hexResult(s) << std::endl;
          s.clear();
        }
    }
   /* Scan R format instruction with 1 arg - SUPPORTS: MFHI & MFLO */ 
   else if ((sscanf(line2,"%s $%[^\n]", oper, rd) == 2) && ((strcmp(oper, "mfhi") == 0) || (strcmp(oper, "mflo") == 0)))
     {
       if (strcmp(oper, "mfhi") == 0)                 // MFHI
       {
       rd_ = locateReg(rd);
       
       /* locally used variables */
       std::stringstream ss;

       /* convert decimals into binary */
       std::bitset<6> opb (0);
       std::bitset<5> rdb (rd_);
       std::bitset<10> hib (0);
       std::bitset<5> shamtb (0);
       std::bitset<6> functb (16);

       /* assign bits to stringstream */
       ss << opb;
       ss << hib;
       ss << rdb;
       ss << shamtb;
       ss << functb;

       /* pass string to hexResult function for hex output */
       std::string s = ss.str();
       ss.clear();
       std::cout << hexResult(s) << std::endl;
       s.clear();
       }
       else if (strcmp(oper, "mflo") == 0)                 // MFLO
       {
         rd_ = locateReg(rd);

         /* locally used variables */
         std::stringstream ss;

         /* convert decimals into binary */
         std::bitset<6> opb (0);
         std::bitset<5> rdb (rd_);
         std::bitset<10> hib (0);
         std::bitset<5> shamtb (0);
         std::bitset<6> functb (18);

         /* assign bits to stringstream */
         ss << opb;
         ss << hib;
         ss << rdb;
         ss << shamtb;
         ss << functb;

         /* pass string to hexResult function for hex output */
         std::string s = ss.str();
         ss.clear();
         std::cout << hexResult(s) << std::endl;
         s.clear();
      }
     }
   
/* Scan for I format instructions with 3 args - SUPPORTS: ADDIU, BEQ, and BNE   */
   if (sscanf(line2, "%s $%[^,],$%[^,],%s", oper, rs, rt, imme) == 4)  {
      
     if (strcmp(oper, "addiu") == 0)                       // ADDIU
      {
        ++counter;
        /* locate registry and assign to int */
        rt_ = locateReg(rt);
        rs_ = locateReg(rs);
        sscanf(imme, "%d", &imm);

        /* locally used variables */
        std::stringstream ss;
        std::string hexResults;

        /* convert decimals into binary */
        std::bitset<6> opb (9);
        std::bitset<5> rsb (rs_);
        std::bitset<5> rtb (rt_);
        std::bitset<16> immb (imm);

        /* assign bits to stringstream */
        ss << opb;
        ss << rtb;
        ss << rsb;
        ss << immb;
        
        /* pass string to hexResult function for hex output */
        std::string s = ss.str();
        ss.clear();
        std::cout << hexResult(s) << std::endl;
        s.clear();
      }
      else if (strcmp(oper, "beq") == 0)                   // BEQ
      {
        ++counter;
        /* locate registry and assign to int */
        rs_ = locateReg(rs);
        rt_ = locateReg(rt);
        imm = 0;

        /* locally used variables */
        std::stringstream ss;
        std::string tempstr = imme;
       
        /* convert decimals into binary */
        std::bitset<6> opb (4);
        std::bitset<5> rsb (rs_);
        std::bitset<5> rtb (rt_);
               
        std::map<std::string,int>::iterator it = mymap.find(tempstr);
        
        if (it != mymap.end())
        {
          imm = it->second;
          imm = imm - counter;
        }
        std::bitset<16> immb (imm);

        /* assign bits to stringstream */
        ss << opb;
        ss << rsb;
        ss << rtb;
        ss << immb;

        /* pass string to hexResult function for hex output */
        std::string s = ss.str();
        ss.clear();
        std::cout << hexResult(s) << std::endl;
        s.clear();
      }
      else if (strcmp(oper, "bne") == 0)                 // BNE
        {
          ++counter;
          /* locate registry and assign to int */
          rs_ = locateReg(rs);
          rt_ = locateReg(rt);
          imm = 0;

          /* locally used variables */
          std::stringstream ss;
          std::string tempstr = imme;

          /* convert decimals into binary */
          std::bitset<6> opb (5);
          std::bitset<5> rsb (rs_);
          std::bitset<5> rtb (rt_);

          std::map<std::string,int>::iterator it = mymap.find(tempstr);

          if (it != mymap.end())
          {
            imm = it->second;
            imm = imm - counter;
          }
          std::bitset<16> immb (imm);

          /* assign bits to stringstream */
          ss << opb;
          ss << rsb;
          ss << rtb;
          ss << immb;

          /* pass string to hexResult function for hex output */
          std::string s = ss.str();
          ss.clear();
          std::cout << hexResult(s) << std::endl;
          s.clear();
        }
   }
   /* Scans I instructions with 3 args and takes variables - SUPPORTS LW & SW */ 
   else if (sscanf(line2, "%s $%[^,],%[^(]($%[^)]", oper, rt, imme, rs) == 4)
   { 
     if (strcmp(oper, "lw") == 0)                                         // LW
      {
        ++counter;
        /* locate registry and assign to int */
        rs_ = locateReg(rs);
        rt_ = locateReg(rt);
        
        /* check if immediate is a variable */
        bool check = false;

        while (imme[i] != '\0')
        { 
          if (isdigit(imme[i]))
            check = true;
          else 
            check = false;
          ++i;
        }

        if (check)
          sscanf(imme, "%d", &imm);
        else 
          imm = n;
        
        /* locate registry and assign to int */        
        std::stringstream ss;
        std::string hexResults;

        /* convert decimals into binary */
        std::bitset<6> opb (35);
        std::bitset<5> rtb (rt_);
        std::bitset<5> rsb (rs_);
        std::bitset<16> immb (imm);

        /* assign bits to stringstream */
        ss << opb;
        ss << rsb;
        ss << rtb;
        ss << immb;

        /* pass string to hexResult function for hex output */
        std::string s = ss.str();
        ss.clear();
        std::cout << hexResult(s) << std::endl;
        s.clear();     
      }
       else if (strcmp(oper, "sw") == 0)                        // SW
      {
        ++counter;
        /* locate registry and assign to int */
        rs_ = locateReg(rs);
        rt_ = locateReg(rt);
        //sscanf(imme, "%d", &imm);

        /* check if immediate is a variable */
        bool check = false;

        while (imme[i] != '\0')
        {
          if (isdigit(imme[i]))
            check = true;
          else
            check = false;
          ++i;
        }
        if (check)
          sscanf(imme, "%d", &imm);
        else
          imm = n;
        
        /* locate registry and assign to int */
        std::stringstream ss;
        std::string hexResults;


        /* convert decimals into binary */
        std::bitset<6> opb (43);
        std::bitset<5> rtb (rt_);
        std::bitset<5> rsb (rs_);
        std::bitset<16> immb (imm);

        /* assign bits to stringstream */
        ss << opb;
        ss << rsb;
        ss << rtb;
        ss << immb;

        /* pass string to hexResult function for hex output */
        std::string s = ss.str();
        ss.clear();
        std::cout << hexResult(s) << std::endl;
        s.clear();
      }
     }
        /* Scans for J instructions with 1 arg - SUPPORTS: J */ 
   if (sscanf(line2, "%s %s", oper, addr) == 2)  {
    
     /* check for jump instruction */
     if (strcmp(oper, "j") == 0)                               // J 
     {
       ++counter;
            
       /* locally used variables */
       std::stringstream ss;
       std::string hexResults;
       std::string tempstr = addr;
       int calcu;

       /* convert decimals into binary */
       std::bitset<6> opb (2);
       
       /* Locates label in map to perform line calculation */ 
       std::map<std::string,int>::iterator it = mymap.find(tempstr);

       if (it != mymap.end())
       {
         calcu = it->second;
         calcu -= 1 ;
       }

       std::bitset<26> addrb (calcu);

       /* assign bits to stringstream */
       ss << opb;
       ss << addrb;

       /* pass string to hexResult function for hex output */
       std::string s = ss.str();
       ss.clear();
       std::cout << hexResult(s) << std::endl;
       s.clear();
     }   
   }
    /* Scan for data */
   else if (sscanf(line2, "%s", oper) == 1)
   {
     /* if .data print hex equiv. */
     if (strcmp(oper,".data") == 0)                          // read data
     {
       if (data)
       {  
         /* local variables */ 
       std::stringstream ss;

       /* convern passed in variable to binary */ 
       std::bitset<32> nb (n);
       
       /* add binary into steam stream */ 
       ss << nb;

       /* pass string to hexResult function for hex output */
       std::string s = ss.str();
       ss.clear();
       std::cout << hexResult(s) << std::endl;  
       s.clear();
       }
     }
     /* Scan for syscalls - SUPPORTS SYSCALL */
     else if (strcmp(oper, "syscall") == 0)                  // syscall
     {
       ++counter;
       
       /* locate registry and assign to int */
       std::stringstream ss;
       std::string hexResults;

       /* convert decimals into binary */
       std::bitset<6> opb (0);
       std::bitset<5> rdb (0);
       std::bitset<5> rsb (0);
       std::bitset<5> rtb (0);
       std::bitset<5> shamtb (0);
       std::bitset<6> functb (12);

       /* assign bits to stringstream */
       ss << opb;
       ss << rsb;
       ss << rtb;
       ss << rdb;
       ss << shamtb;
       ss << functb;

       /* pass string to hexResult function for hex output */
       std::string s = ss.str();
       ss.clear();
       std::cout << hexResult(s) << std::endl;
       s.clear();
     }
   }
     /* if not mips instruction print out incorrect format */    
    else {
      printf("input line: %s\n", line2);      
      printf("Incorrect format.\n");
    }
  }
  return 0;
}

std::string hexResult(std::string s)
{
  /* local variables*/
  std::string result = "";
  int hold = 0;
  int i = 0;
  int hex1 = 0;
  int hex2 = 0;
  int hex3 = 0;
  int hex4 = 0;
  int hex5 = 0;
  int hex6 = 0;
  int hex7 = 0;
  int hex8 = 0;
  int to = 0; 

  std::string b1 = "";
  std::string b2 = "";
  std::string b3 = "";
  std::string b4 = "";
  std::string b5 = "";
  std::string b6 = "";
  std::string b7 = "";
  std::string b8 = "";

  /* split 32 binary string into 4 bit strings */ 
  while (i < s.size())
  {
    if (i < 4)
      b1 += s[i];
    else if (i >= 4 && i < 8)
      b2 += s[i];
    else if (i >= 8 && i < 12)
      b3 += s[i];
    else if (i >= 12 && i < 16)
      b4 += s[i];
    else if (i >= 16 && i < 20)
      b5 += s[i];
    else if (i >= 20 && i < 24)
      b6 += s[i];
    else if (i >= 24 && i < 28)
      b7 += s[i];
    else if (i >= 28 && i < 32)
      b8 += s[i];
    ++i;
  }
  /* calculate bit binary index */
  for (int x = 3; 0 <= x; --x)
  {
    if (b1[x] == '1')
      hex1 += pow(2,to);

    if (b2[x] == '1')
      hex2 += pow(2,to);
    
    if (b3[x] == '1')
      hex3 += pow(2,to);

    if (b4[x] == '1')
      hex4 += pow(2,to);

    if (b5[x] == '1')
      hex5 += pow(2,to);

    if (b6[x] == '1')
      hex6 += pow(2,to);

    if (b7[x] == '1')
      hex7 += pow(2,to);
    
    if (b8[x] == '1')
      hex8 += pow(2,to);
    ++to;
  } 
 /* convert bits to hex and store in string stream */
  std::stringstream st;
  result.clear();
  st << std::hex << hex1;
  hex1 = 0;
  st << std::hex << hex2;
  hex2 = 0;
  st << std::hex << hex3;
  hex3 = 0; 
  st << std::hex << hex4;
  hex4 = 0;
  st << std::hex << hex5;
  hex5 = 0;
  st << std::hex << hex6;
  hex6 = 0;
  st << std::hex << hex7;
  hex7 = 0; 
  st << std::hex << hex8;
  hex8 = 0;
  st >> result;
  to = 0;

  /* return hex results */
  return result;
}  

char locateReg(char *lookfor)
{
  /* look for appropriate registers */ 
  char s = lookfor[0];

  if (lookfor[0] == 's' && lookfor[1] == 'p')
    s = 'q';

  switch (s)
  {
    case 'z':
      {
        return 0;
      }
    case 'v':
      {
        if (lookfor[1] == '0')
          return 2;
        else if (lookfor[1] == '1')
          return 3;
      }
    case 'a':
      {
        if(lookfor[1] == '0')
          return 4;
        else if (lookfor[1] == '1')
          return 5;
        else if (lookfor[1] == '2')
          return 6;
        else if (lookfor[1] == '3')
          return 7;
      }
    case 't':
      {
        if (lookfor[1] == '0')
          return 8;
        else if (lookfor[1] == '1')
          return 9;
        else if (lookfor[1] == '2')
          return 10;
        else if (lookfor[1] == '3')
          return 11;
        else if (lookfor[1] == '4')
          return 12;
        else if (lookfor[1] == '5')
          return 13;
        else if (lookfor[1] == '6')
          return 14;
        else if (lookfor[1] == '7')
          return 15;
        else if (lookfor[1] == '8')
          return 24;
        else if (lookfor[1] == '9')
          return 25;
      }
    case 's':
      {
        if (lookfor[1] == '0')
          return 16;
        else if (lookfor[1] == '1')
          return 17;
        else if (lookfor[1] == '2')
          return 18;
        else if (lookfor[1] == '3')
          return 19;
        else if (lookfor[1] == '4')
          return 20;
        else if (lookfor[1] == '5')
          return 21;
        else if (lookfor[1] == '6')
          return 22;
        else if (lookfor[1] == '7')
          return 23;
      }
    case 'k':
      {
        if (lookfor[1] == '0')
          return 26;
        else if (lookfor[1] == '1')
          return 27;
      }
    case 'g':
      {
        return 28;
      }
    case 'q':
      {
        return 29;
      }
    case 'f':
      {
        return 30;
      }
    case 'r':
      {
        return 31;
      }
    default: return 0;
  }
}
