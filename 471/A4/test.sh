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
	sleep 3
done

g++ -o echo a4*

./echo