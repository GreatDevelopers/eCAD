astyle --options=./ecad.astylerc --recursive  "*.cpp" "*.h"

find . -name "*.orig" -exec rm {} \;
