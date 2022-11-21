#!/bin/bash

source utils.sh --source-specific get_default_args get_conf_param

echo -n "" > README.md

while IFS="" read -r LINE || [ -n "$LINE" ]
do
    if [[ "${LINE}" == *"header"* ]] ; then
        echo '' >> README.md
        echo '# [C++] APPLICATION with NEAT and TidyVolve' >> README.md
        echo '' >> README.md
        echo '- My very own NEAT framework in C++ (it'"'"'s v2 with SFML) : https://github.com/romainducrocq/NEAT-TidyV2fml' >> README.md
    elif [[ "${LINE}" == *"how_to_run"* ]] ; then
        echo "${LINE}" >> README.md
        echo '```' >> README.md
        echo '$ ./build.sh -R' >> README.md
        echo '$ ./compile.sh' >> README.md
        echo '$ ./run.sh -m train '"$(get_default_args 'TRAIN')" >> README.md
        echo '$ ./run.sh -m eval '"$(get_default_args 'EVAL')" >> README.md
        echo '```' >> README.md
        echo '</span>' >> README.md
    elif [[ "${LINE}" == *"how_to_train"* ]] ; then
        echo "${LINE}" >> README.md
        echo '```' >> README.md
        echo '* Debug   : $ ./train.sh -D '"$(get_default_args 'TRAIN')" >> README.md
        echo '* Release : $ ./train.sh -R '"$(get_default_args 'TRAIN')" >> README.md
        echo '```' >> README.md
        echo '</span>' >> README.md
    elif [[ "${LINE}" == *"how_to_eval"* ]] ; then
        echo "${LINE}" >> README.md
        echo '```' >> README.md
        echo '* Debug   : $ ./eval.sh -D '"$(get_default_args 'EVAL')" >> README.md
        echo '* Release : $ ./eval.sh -R '"$(get_default_args 'EVAL')" >> README.md
        echo '```' >> README.md
        echo '</span>' >> README.md
    elif [[ "${LINE}" == *"how_to_play"* ]] ; then
        echo "${LINE}" >> README.md
        echo '```' >> README.md
        echo '* Debug   : $ ./play.sh -D '"$(get_default_args 'PLAY')" >> README.md
        echo '* Release : $ ./play.sh -R '"$(get_default_args 'PLAY')" >> README.md
        echo '```' >> README.md
        echo '</span>' >> README.md
    elif [[ "${LINE}" == *"how_to_test"* ]] ; then
        echo "${LINE}" >> README.md
        echo '```' >> README.md
        echo '* Debug   : $ ./test.sh -D '"$(get_default_args 'TEST')" >> README.md
        echo '* Release : $ ./test.sh -R '"$(get_default_args 'TEST')" >> README.md
        echo '```' >> README.md
        echo '</span>' >> README.md
    elif [[ "${LINE}" == *"hyperparameter_values"* ]] ; then
        echo "${LINE}" >> README.md
        echo '```' >> README.md
        echo '* HYPERPARAMETER VALUES :' >> README.md
        echo '' >> README.md
        echo 'INPUTS               = '"$(get_conf_param INPUTS)" >> README.md
        echo 'OUTPUTS              = '"$(get_conf_param OUTPUTS)" >> README.md
        echo '' >> README.md
        echo 'LIM_HIDDEN           = '"$(get_conf_param LIM_HIDDEN)" >> README.md
        echo '' >> README.md
        echo 'MUTATE_WEIGHT_RATE   = '"$(get_conf_param MUTATE_WEIGHT_RATE)" >> README.md
        echo 'MUTATE_GENE_RATE     = '"$(get_conf_param MUTATE_GENE_RATE)" >> README.md
        echo 'MUTATE_LINK_RATE     = '"$(get_conf_param MUTATE_LINK_RATE)" >> README.md
        echo 'MUTATE_BIAS_RATE     = '"$(get_conf_param MUTATE_BIAS_RATE)" >> README.md
        echo 'MUTATE_NEURON_RATE   = '"$(get_conf_param MUTATE_NEURON_RATE)" >> README.md
        echo 'MUTATE_ENABLE_RATE   = '"$(get_conf_param MUTATE_ENABLE_RATE)" >> README.md
        echo 'MUTATE_DISABLE_RATE  = '"$(get_conf_param MUTATE_DISABLE_RATE)" >> README.md
        echo 'MUTATE_OFFSET_SIZE   = '"$(get_conf_param MUTATE_OFFSET_SIZE)" >> README.md
        echo 'MUTATE_RATE_DECAY    = '"$(get_conf_param MUTATE_RATE_DECAY)" >> README.md
        echo '' >> README.md
        echo 'DELTA_DISJOINT       = '"$(get_conf_param DELTA_DISJOINT)" >> README.md
        echo 'DELTA_WEIGHTS        = '"$(get_conf_param DELTA_WEIGHTS)" >> README.md
        echo 'DELTA_THRESHOLD      = '"$(get_conf_param DELTA_THRESHOLD)" >> README.md
        echo '' >> README.md
        echo 'CROSSOVER_PROB       = '"$(get_conf_param CROSSOVER_PROB)" >> README.md
        echo 'STALE_SPECIES        = '"$(get_conf_param STALE_SPECIES)" >> README.md
        echo 'POPULATION_SIZE      = '"$(get_conf_param POPULATION_SIZE)" >> README.md
        echo 'MAX_POPULATION_SIZE  = '"$(get_conf_param MAX_POPULATION_SIZE)" >> README.md
        echo 'POPULATION_GENS_INC  = '"$(get_conf_param POPULATION_GENS_INC)" >> README.md
        echo 'POPULATION_INC_FREQ  = '"$(get_conf_param POPULATION_INC_FREQ)" >> README.md
        echo '' >> README.md
        echo 'ACT_REPEAT           = '"$(get_conf_param ACT_REPEAT)" >> README.md
        echo 'MVG_AVG              = '"$(get_conf_param MVG_AVG)" >> README.md
        echo 'PLT_FREQ             = '"$(get_conf_param PLT_FREQ)" >> README.md
        echo 'SAV_FREQ             = '"$(get_conf_param SAV_FREQ)" >> README.md
        echo '' >> README.md
        echo 'GENERATIONS_TRAIN    = '"$(get_conf_param GENERATIONS_TRAIN)" >> README.md
        echo 'EPOCHS_EVAL          = '"$(get_conf_param EPOCHS_EVAL)" >> README.md
        echo 'MAX_STEP             = '"$(get_conf_param MAX_STEP)" >> README.md
        echo 'MAX_NOOP             = '"$(get_conf_param MAX_NOOP)" >> README.md
        echo '```' >> README.md
        echo '</span>' >> README.md
    elif [[ "${LINE}" == *"plot_demo"* ]] ; then
        echo "${LINE}" >> README.md
        echo '  ' >> README.md
        if [ -f "../log/plots/demo.png" ] ; then
            echo '![Demo](log/plots/demo.png)' >> README.md
        fi
        echo '  ' >> README.md
        echo '</span>' >> README.md
    elif [[ "${LINE}" == *"gif_demo"* ]] ; then
        echo "${LINE}" >> README.md
        echo '  ' >> README.md
        if [ -f "../res/gif/demo.gif" ] ; then
            echo '![Demo](res/gif/demo.gif)' >> README.md
        fi
        echo '  ' >> README.md
        echo '</span>' >> README.md
    fi

    if [[ "${LINE}" == *"span"* ]] ; then
        while IFS="" read -r LINE || [ -n "$LINE" ]
        do
            if [[ "${LINE}" == *"span"* ]] ; then
                break
            fi
        done
    else
        echo "${LINE}" >> README.md
    fi

done < ../README.md

mv README.md ..
