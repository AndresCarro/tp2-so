docker start arqui
docker exec -it arqui make clean -C /root/Toolchain
docker exec -it arqui make clean -C /root/
if [[ "$1" != "clean" ]]
then
    docker exec -it arqui make -C /root/Toolchain
    docker exec -it arqui make -C /root/
fi
docker stop arqui