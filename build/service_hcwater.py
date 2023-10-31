# -*- coding:utf-8 -*-

import os
import time
from flask import Flask
from flask import request, jsonify

app = Flask(__name__)

appstr = "--appname=water_bloom_monitor_extract" 
retstatus = ""

def build_para_str(tmpstr):
    retstr = "--"+tmpstr+"="
    return retstr

def request_parse(req_data):
    if req_data.method == 'POST':
        data = req_data.get_json()
    elif req_data.method == 'GET':
        data = req_data.args
    return data
	
@app.route('/blackwater_detect', methods=['GET','POST'])
def blackwater_detect():
    data = request_parse(request)
    inputfile = data['inputImage']
    outputdir = data['outputImage']
    
    basefile = os.path.basename(inputfile)
    basename,suffix = os.path.splitext(basefile)

    current_timestamp = round(time.time()*1000)
    outputfile = os.path.join(outputdir, basename+"_"+str(current_timestamp)+suffix)
    
    inputImage = build_para_str("inputImage") + inputfile
    outputImage = build_para_str("outputImage") + outputfile
    satelliteID = build_para_str("satelliteID") + data['satelliteID']

    commdline = './GEBAlgProcess ' + appstr + ' ' + inputImage + ' ' + outputImage + ' ' + satelliteID
    print('commdline:',commdline); 

    retcode = os.system(commdline)
    if retcode == 0:
        retstatus = "success"
    else:
        retstatus = "failed"
        
    return_info = {'result': outputfile, 'status': retstatus}
    return jsonify({'outputs': return_info})
	
if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5000,debug=True)

