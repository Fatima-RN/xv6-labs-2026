with open("kernel/syscall.c", "r") as f:
    content = f.read()

# Replace the broken argint function cleanly
old_argint = """void
argint(int n, int *ip)
{
  struct proc *p = myproc();
  if(argraw(n, ip)<0)
      return -1;
   return 0;
}"""

new_argint = """int
argint(int n, int *ip)
{
  return argraw(n, ip);
}"""

if old_argint in content:
    content = content.replace(old_argint, new_argint)
else:
    # Just find and replace starting from argint definition if formatting varies slightly
    import re
    content = re.sub(r'void\s*\n\s*argint\(int n, int \*ip\).*?\}', new_argint, content, flags=re.DOTALL)

with open("kernel/syscall.c", "w") as f:
    f.write(content)
print("Updated syscall.c successfully!")
