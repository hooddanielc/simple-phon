DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

echo $DIR

$DIR/../ib/ib --src_root $DIR/../lib-simple-phon --cfg_root $DIR/../lib-simple-phon /main-osx --force --out_root $DIR/../out/lib-simple-phon
