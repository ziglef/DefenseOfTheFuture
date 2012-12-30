echo "Compiling and Installing the Program"
make clean install

wait

echo "Striping the Executable"
strip -s /usr/sbin/dotf

wait

echo "Executing the Program"
service run /usr/sbin/dotf
