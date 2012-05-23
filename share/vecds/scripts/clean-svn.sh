make clean
find . -name '.svn' -print0 | xargs -0 rm -rf
find . -name '*~'   -print0 | xargs -0 rm -f

