#~/bin/bash
for file in $(ls -1); do
	if [[ "$file" = "echo" ]]; then
		rm $file
	elif [[ "$file" = "a4skel.cpp" ]]; then
		rm $file
	fi
done

ls
scp -i ~/.ssh/id_rsa rza31@cs-vnl:~/a4* .
while [[ "$(ls -1|wc -l)" -lt 3 ]]; do
	sleep 2
done

g++ -o echo a4*

if [[ $? -ne 0 ]]; 
then
	rm a4*
	echo "make failed"
else
	rm a4*
	./echo 172.17.1.8 172.19.1.18
fi
