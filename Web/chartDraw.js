function addBar(config, backColor, xMIN,xMAX,yMIN,yMAX)
{
	if (!config.hasOwnProperty("options")) 
	{
		config.push
		(
		{

			options: 
			{
				plugins: 
				{
					autocolors: false,
					annotation: 
					{
						annotations: 
						{

						}

					}
				}
			}
		}
		);
	}
	if (!config.hasOwnProperty("AnoIndex")) 
	{
		config.AnoIndex=0;
	}
	else 
	{
		config.AnoIndex++;
	}
// config.options.plugins.annotation.annotations.box3=[];
config.options.plugins.annotation.annotations[config.AnoIndex]=		 
{
	drawTime: 'beforeDatasetsDraw',
	type: 'box',
	xMin: xMIN,
	xMax: xMAX,
	yMin: yMIN,
	yMax: yMAX,
	backgroundColor: backColor
};
	//config.options.plugins.annotation.annotations.push({key: "keys"});
	// console.log(config);
	return config;
}

function addXLine(config, borderColor,borderWidth,yMIN,yMAX)
{
	if (!config.hasOwnProperty("options")) 
	{
		config.push
		(
		{

			options: 
			{
				plugins: 
				{
					autocolors: false,
					annotation: 
					{
						annotations: 
						{

						}

					}
				}
			}
		}
		);
	}
	if (!config.hasOwnProperty("AnoIndex")) 
	{
		config.AnoIndex=0;
	}
	else 
	{
		config.AnoIndex++;
	}
// config.options.plugins.annotation.annotations.box3=[];
config.options.plugins.annotation.annotations[config.AnoIndex]=		 
{
	drawTime: 'beforeDatasetsDraw',
	type: 'line',
	yMin: yMIN,
	yMax: yMAX,
	borderColor: borderColor,
	borderWidth: borderWidth,				
};
	//config.options.plugins.annotation.annotations.push({key: "keys"});
	// console.log(config);
	return config;
}


function drawLineGraf(ctx) {
	//alert("test");
	const DATA_COUNT = 15;
	const labels = [];
	for (let i = 0; i < DATA_COUNT; ++i) {
		labels.push(i.toString());
	}
	const datapoints = [-10, 20, 20, 60, 80, 120, 140, 180, 120, 125, 105, 110, 170, -10];
	const data = {
		labels: labels,
		datasets: [
		{
			label: 'Cubic interpolation (monotone)',
			data: datapoints,
			borderColor: 'rgba(255, 0, 0, 1)',
			fill: false,
			cubicInterpolationMode: 'monotone',
			tension: 0.1
		}
		]
	};

	config = 
	{
		type: 'line',
		data: data,
	    options: 
	    {
	    	plugins: 
	    	{
	    		autocolors: false,
	    		annotation: 
	    		{
	    			annotations: 
	    			{		

	    			}

	    		}
	    	}
	    }
	};
addBar(config,'rgba(2, 14, 132, 0.25)'   ,1,3,70,140);
addBar(config,'rgba(255, 255, 0, 0.5)'   ,2,3,70,90);
addBar(config,'rgba(255, 99, 132, 0.25)' ,1,2,50,70);

addXLine(config,'rgb(255, 99, 132)', 2,90,90);

var myChart = new Chart(ctx, config);

return myChart;
}