#! /bin/sh

#------------------------ simulazione 1: st=2 h=0 ----------------------------------
TEMP=2.0

mkdir ../OUTPUT/sim_type_2_h_0 ../OUTPUT/sim_type_3_h_0 ../OUTPUT/sim_type_2_h_0.02 ../OUTPUT/sim_type_3_h_0.02

sed -i "1s/.*/SIMULATION_TYPE        2    1.0    0.0/" ../INPUT/input.dat

for i in $(seq 0 15); do
    
    sed -i "3s/.*/TEMP \t\t\t\t   ${TEMP}/" ../INPUT/input.dat

    ./simulator.exe

    mv ../OUTPUT/specific_heat.dat "../OUTPUT/sim_type_2_h_0/specific_heat${i}.dat"

    mv ../OUTPUT/susceptibility.dat "../OUTPUT/sim_type_2_h_0/susceptibility${i}.dat"

    mv ../OUTPUT/total_energy.dat "../OUTPUT/sim_type_2_h_0/total_energy${i}.dat"

    cp ../OUTPUT/CONFIG/config.spin ../INPUT/CONFIG/config.spin
    if [ $i -eq 0 ]; then
        sed -i "2s/.*/RESTART \t\t\t   1/" ../INPUT/input.dat
    fi

    if [ $i -eq 9 ]; then
        sed -i "9s/.*/NSTEPS  \t\t\t   40000/" ../INPUT/input.dat
    fi

    TEMP=$(awk "BEGIN {print $TEMP - 0.1}")
done

sed -i "2s/.*/RESTART \t\t\t   0/" ../INPUT/input.dat

sed -i "9s/.*/NSTEPS  \t\t\t   20000/" ../INPUT/input.dat

#------------------------ simulazione 2: st=3 h=0 ----------------------------------

TEMP=2.0

sed -i "1s/.*/SIMULATION_TYPE        3    1.0    0.0/" ../INPUT/input.dat

for i in $(seq 0 15); do
    
    sed -i "3s/.*/TEMP \t\t\t\t   ${TEMP}/" ../INPUT/input.dat

    ./simulator.exe

    mv ../OUTPUT/specific_heat.dat "../OUTPUT/sim_type_3_h_0/specific_heat${i}.dat"

    mv ../OUTPUT/susceptibility.dat "../OUTPUT/sim_type_3_h_0/susceptibility${i}.dat"

    mv ../OUTPUT/total_energy.dat "../OUTPUT/sim_type_3_h_0/total_energy${i}.dat"

    cp ../OUTPUT/CONFIG/config.spin ../INPUT/CONFIG/config.spin
    
    if [ $i -eq 0 ]; then
        sed -i "2s/.*/RESTART \t\t\t   1/" ../INPUT/input.dat
    fi

    if [ $i -eq 9 ]; then
        sed -i "9s/.*/NSTEPS  \t\t\t   40000/" ../INPUT/input.dat
    fi

    TEMP=$(awk "BEGIN {print $TEMP - 0.1}")
done

sed -i "2s/.*/RESTART \t\t\t   0/" ../INPUT/input.dat

sed -i "9s/.*/NSTEPS  \t\t\t   20000/" ../INPUT/input.dat

#------------------------ simulazione 3: st=2 h=0.02 ----------------------------------

TEMP=2.0

sed -i "1s/.*/SIMULATION_TYPE        2    1.0    0.02/" ../INPUT/input.dat

for i in $(seq 0 15); do
    
    sed -i "3s/.*/TEMP \t\t\t\t   ${TEMP}/" ../INPUT/input.dat

    ./simulator.exe

    mv ../OUTPUT/magnetization.dat "../OUTPUT/sim_type_2_h_0.02/magnetization${i}.dat"

    cp ../OUTPUT/CONFIG/config.spin ../INPUT/CONFIG/config.spin
    
    if [ $i -eq 0 ]; then
        sed -i "2s/.*/RESTART \t\t\t   1/" ../INPUT/input.dat
    fi

    if [ $i -eq 9 ]; then
        sed -i "9s/.*/NSTEPS  \t\t\t   40000/" ../INPUT/input.dat
    fi

    TEMP=$(awk "BEGIN {print $TEMP - 0.1}")
done

sed -i "2s/.*/RESTART \t\t\t   0/" ../INPUT/input.dat

sed -i "9s/.*/NSTEPS  \t\t\t   20000/" ../INPUT/input.dat

#------------------------ simulazione 4: st=3 h=0.02 ----------------------------------

TEMP=2.0

sed -i "1s/.*/SIMULATION_TYPE        3    1.0    0.02/" ../INPUT/input.dat

for i in $(seq 0 15); do
    
    sed -i "3s/.*/TEMP \t\t\t\t   ${TEMP}/" ../INPUT/input.dat

    ./simulator.exe

    mv ../OUTPUT/magnetization.dat "../OUTPUT/sim_type_3_h_0.02/magnetization${i}.dat"
    
    cp ../OUTPUT/CONFIG/config.spin ../INPUT/CONFIG/config.spin
    
    if [ $i -eq 0 ]; then
        sed -i "2s/.*/RESTART \t\t\t   1/" ../INPUT/input.dat
    fi

    if [ $i -eq 9 ]; then
        sed -i "9s/.*/NSTEPS  \t\t\t   40000/" ../INPUT/input.dat
    fi

    TEMP=$(awk "BEGIN {print $TEMP - 0.1}")
done

sed -i "2s/.*/RESTART \t\t\t   0/" ../INPUT/input.dat

sed -i "9s/.*/NSTEPS  \t\t\t   20000/" ../INPUT/input.dat