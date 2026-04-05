awk '{if($1=="forward"){x+=$2}; if($1=="down"){y+=$2}; if($1=="up"){y-=$2}} END {print x*y}' $1

awk '{if($1=="forward"){x+=$2; y+=aim*$2}; if($1=="down"){aim+=$2}; if($1=="up"){aim-=$2}} END {print x*y}' $1