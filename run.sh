# Definir el directorio de salida para los archivos de parámetros y los archivos de resultados
dir_output_params="params"
dir_output_results="results"

# Rango para el tiempo promedio de llegada
media_entre_llegadas_min=1
media_entre_llegadas_max=1000

# Rango para el tiempo promedio de atención
media_atencion_min=0
media_atencion_max=10

# Rango para el número de clientes esperados
num_esperas_requerido_min=10
num_esperas_requerido_max=100

# Rango para el número de servidores
num_servidores_min=1
num_servidores_max=50

# Crear los directorios si no existen
mkdir -p $dir_output_params
mkdir -p $dir_output_results

# Crear los archivos de entrada
for i in {1..10}
do
  media_entre_llegadas=$(( media_entre_llegadas_min + ((media_entre_llegadas_max - media_entre_llegadas_min) * i) / 10 ))
  media_atencion=$(( media_atencion_min + ((media_atencion_max - media_atencion_min) * i) / 10 ))
  num_esperas_requerido=$(( num_esperas_requerido_min + ((num_esperas_requerido_max - num_esperas_requerido_min) * i) / 10 ))
  num_servidores=$(( num_servidores_min + ((num_servidores_max - num_servidores_min) * i) / 10 ))

  echo "$media_entre_llegadas 
$media_atencion 
$num_esperas_requerido 
$num_servidores" > "${dir_output_params}/param$i.txt"
done

# Compilamos el programa una vez
g++ -o Erlang Sistema_mmm.cpp moduls/inicializar.cpp moduls/controltiempo.cpp moduls/llegada.cpp moduls/salida.cpp moduls/reportes.cpp moduls/actualizar_estad_prom_tiempo.cpp

# Iteramos sobre cada archivo de parámetros en el directorio
for param_file in ${dir_output_params}/param*.txt; do
  # Creamos un nombre para el archivo de resultados basado en el nombre del archivo de parámetros
  result_file="${dir_output_results}/${param_file/${dir_output_params}\/param/result}"

  # Ejecutamos la simulación con los parámetros actuales y dirigimos la salida al archivo de resultados correspondiente
  ./Erlang "$param_file" "$result_file"
done
