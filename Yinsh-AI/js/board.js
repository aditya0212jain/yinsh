rows = 11
rings = 5

var positions = new Array(rows);
for (var i = 0; i < rows; i++) {
  positions[i] = new Array(rows);
}

function Point(x, y) {
  this.x = x;
  this.y = y;
  this.piece=0;  //2-> player 0, -2-> player 1
  this.guide=false;
}

function PlotPoints(){
	for (var i = 0; i < rows; i++) {
		var x=i-rings;
		var low=-rings;
		var high=rings;

		if(x==0){
			low=-(rings-1); high=(rings-1);
		}

		if(x>=1&&x<=(rings-1)){
			low=-(rings)+x;
		}

		if(x==(rings)){
			low=1; high=(rings-1);
		}

		if(x>=-(rings-1)&&x<=-1){
			high=rings+x;
		}

		if(x==-(rings)){
			low=-(rings-1); high=-1;
		}

		for(var j=0;j<rows;j++){
			var y=j-(rings);

			if(!(y>=low&&y<=high)){
				positions[i][j]= new Point(-1,-1);
        console.log("in the neglect "+i+" "+j);
				continue;
			}
      console.log("valid point ("+i+","+j+")")
			positions[i][j]= new Point(x,y);
			positions[i][j].valid=true;
		}
	}
}

PlotPoints();
