google.charts.load('current', {packages: ['corechart', 'line']});
google.charts.setOnLoadCallback(drawBasic);

function drawBasic() {

           var data = new google.visualization.DataTable();
      data.addColumn('timeofday', 'Time of Day');
      data.addColumn('number', 'Probe1');
      data.addColumn('number', 'Probe2');
      data.addColumn('number', 'Probe3');
      data.addColumn('number', 'Probe4');

      data.addRows([
[[0,00,36], 65, 65, 22.40, 21.06],
[[0,01,06], 65, 65, 22.37, 21.08],
[[23,48,54], 65, 65, 22.74, 21.14],
[[23,49,24], 65, 65, 22.34, 21.16],
[[23,49,54], 65, 65, 22.47, 21.10],
[[23,50,24], 65, 65, 22.38, 21.10],
[[23,50,54], 65, 65, 22.48, 21.19],
[[23,51,24], 65, 65, 22.45, 21.14],
[[23,51,54], 65, 65, 22.51, 21.16],
[[23,52,24], 65, 65, 22.49, 21.17]
      ]);

      var options = {
        hAxis: {
          title: 'Time'
        },
        vAxis: {
          title: 'Popularity'
        }
      };

      var chart = new google.visualization.LineChart(document.getElementById('chart_div'));

      chart.draw(data, options);
    }