for i in {0..9}
do 
    cat testes/caso-0${i}.in | ./main > out/my${i}.out
done

for i in {0..9}
do
    echo ${i}
    diff testes/caso-0${i}.out out/my${i}.out
done