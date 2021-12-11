const { app, ipcRenderer } = require('electron');
const fs = require('fs')
const path = require('path');
const axios = require('axios')
const child = require('child_process')


//Create a map and center it on Maryland
var mymap = L.map('mapid').setView([39.29, -76.641], 6);

//Create variables that will be used by various functions
var curLayer = null;
var curName = "original"
var featureGroup = L.layerGroup().addTo(mymap)
var isProcessing = false

//Add the map tiles to the map object
L.tileLayer('https://api.mapbox.com/styles/v1/{id}/tiles/{z}/{x}/{y}?access_token={accessToken}', {
    attribution: '© <a href="https://www.mapbox.com/about/maps/">Mapbox</a> © <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a> <strong><a href="https://www.mapbox.com/map-feedback/" target="_blank">Improve this map</a></strong>',
    maxZoom: 18,
    id: 'mapbox/streets-v11',
    tileSize: 512,
    zoomOffset: -1,
    accessToken: 'pk.eyJ1Ijoia2lsbGpveTk5OTkiLCJhIjoiY2s3YXY1bmJoMDFpbTNxcWdndXJrYnczbSJ9.5vhlhPIFexk3nAL_5Li_MQ'
}).addTo(mymap);


//Loads the file onto the map
function getFile(fileName, label_method){
    axios.get(path.join(__dirname, fileName))
    .then(res => res.data)
    .then(jsontext => {
        //If there'are already a layer, remove it
        if(curLayer != null){
            featureGroup.clearLayers();
        }

        if(label_method == "algo1"){

            completeBounds = []
            //Create new geoJSON with properties
            curLayer = new L.geoJson(jsontext, {
                onEachFeature: function(feature, layer){
                    if(!completeBounds.includes(feature.properties.dist_num)){
                        var label = L.tooltip()
                        label.setLatLng(layer.getBounds().getCenter())
                        label.setContent(String(feature.properties.dist_num))
                        label.addTo(featureGroup)
                        completeBounds.push(feature.properties.dist_num)
                    }

                }
            })

        } else if(label_method == "algo2"){
            averageCenters = [[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0],[0,0]]
            counts = [0,0,0,0,0,0,0,0]
            //Create new geoJSON with properties
            curLayer = new L.geoJson(jsontext, {
                //Get center of each polygon in district, add together so we can get avg.
                onEachFeature: function(feature, layer){
                    var lat = layer.getBounds().getCenter().lat
                    var lng = layer.getBounds().getCenter().lng
                    var dist = feature.properties.dist_num-1
                    averageCenters[dist][0] = averageCenters[dist][0] + lat
                    averageCenters[dist][1] = averageCenters[dist][1] + lng
                    counts[dist] = counts[dist] + 1
                }
            })

            //Get average center of each district and place label there.
            for(i = 0; i < 8; i++){
                var lat = averageCenters[i][0] / counts[i]
                var lng = averageCenters[i][1] / counts[i]
                var pos = L.latLng(lat,lng)
            
                var label = L.tooltip()
                label.setLatLng(pos)
                label.setContent(String(i+1))
                label.addTo(featureGroup)
            }

        } else {
            // These are the original districts, so we can hardcode label positions
            var positions = [[39.048,-75.882],[39.348,-76.410], [39.139, -76.95], 
                [38.881, -76.828], [38.343, -76.646], [39.64, -77.73], 
                [39.302, -76.895], [39.502, -77.198]]
                
            curLayer = new L.geoJson(jsontext);

            for(i = 0; i < 8; i++){
                var pos = L.latLng(positions[i][0], positions[i][1])
                var label = L.tooltip()
                label.setLatLng(pos)
                label.setContent(String(i+1))
                label.addTo(featureGroup)
            }
        }

        //Set style of layer
        curLayer.setStyle(function (feature){
            return {color: feature.properties.fill, weight: feature.properties.stroke}
        })

        
        //Add layer to group, which is on map
        curLayer.addTo(featureGroup)


        

        // Adjust map to show the kml
        const bounds = curLayer.getBounds();
        mymap.fitBounds(bounds);

    });
}

//Load initial data
getFile('data_files/current_mcd.geojson', "original")

//Load original files
function show_original(){
    //If already set or an algorithm is running, do nothing
    if(curName == "original" || isProcessing == true){
        return;
    }
    isProcessing = true
    getFile('data_files/current_mcd.geojson', "original")
    curName = "original"
    isProcessing = false
}

//Run algorithm 1
function call_algo1(){
    //If already set or an algorithm is running, do nothing
    if(curName == "algo1" || isProcessing == true){
        return
    }

    //Tell NodeJS to run the python code & display it when done.
    isProcessing = true
    ipcRenderer.invoke('call_algo').then((result) =>{
        var proc = child.spawn('python', [path.join(result, '..', '/data_files/Algorithm1/redistricter.py')])
        proc.on('exit', (code, sig) => {
            getFile('data_files/Algorithm1/md_c.geojson', "algo1")
            curName = "algo1"
            isProcessing = false
        });
    })

    
}


//Run algorithm 2
function call_algo2(){
    //If already set or an algorithm is running, do nothing
    if(curName == "algo2" || isProcessing == true){
        return
    }

    //Tell NodeJS to run the python code & display it when done.
    isProcessing = true
    ipcRenderer.invoke('call_algo').then((result) =>{
        var proc = child.spawn('python', [path.join(result, '..', '/data_files/Algorithm2/k_means_bfs.py')])
        proc.on('exit', (code, sig) => {
            getFile('data_files/Algorithm2/k_means_bfs.geojson', "algo2")
            curName = "algo2"
            isProcessing = false
        });
    })
}

//Display help
function algo1_help(){
    document.getElementById("district-description").innerHTML =
    `Algorithm 1 Description:<br>
    This algorithm groups the 24 Maryland counties into 8 groups of 3 contiguous counties. It does not have any regard for population.
    `
}
//Display help
function algo2_help(){
    document.getElementById("district-description").innerHTML =
    `Algorithm 2 Description:<br>
    This algorithm groups the census tracts in Maryland into 8 (mostly) contiguous groups using K-Means clustering with population balancing considerations. It then refines these districts with BFS to reduce fragmentation.
    `
}