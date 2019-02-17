//
//  ViewController.swift
//  detectedPeople
//
//  Created by Wii on 2/16/19.
//  Copyright © 2019 JinliangWii. All rights reserved.
//

import UIKit

import CocoaAsyncSocket

class ViewController: UIViewController, GCDAsyncUdpSocketDelegate {

    var count = 0
    
    let emotion_string = ["Happy", "Netural", "Bad"]
    
    func toInt32(_ bytes: Data) -> Int32 {
        return Int32(bytes[0]) + Int32(bytes[1]) * 256 + Int32(bytes[2]) * 256 * 256 + Int32(bytes[3]) * 256 * 256 * 256
    }
    
    func udpSocket(_ sock: GCDAsyncUdpSocket, didReceive data: Data, fromAddress address: Data, withFilterContext filterContext: Any?) {
        
        //sock.send(data, toHost: "127.0.0.1", port: 4243, withTimeout: 10, tag: 1)
        
//        let backToString = String(data: data, encoding: String.Encoding.utf8) as String!
//
//        count += 1
//        print(backToString!, count)
//
//        DispatchQueue.main.async {
//            self.name.text = backToString!
//        }
//        DispatchQueue.main.async {
//            self.avatar.image = UIImage(data: data)
//        }
        
        if(data[0] == 0xA1){
            //print("Info Update")
            
            
            let _timestamp = toInt32(data.subdata(in: Range(4..<8)))
            let flag = toInt32(data.subdata(in: Range(8..<12)))
            let _id = toInt32(data.subdata(in: Range(12..<16)))
            let  name_len = toInt32(data.subdata(in: Range(16..<20)))
            let name_end_index = Int(20 + Int(name_len))
            let _name = String(data: data.subdata(in: Range(20..<name_end_index)), encoding: String.Encoding.utf8) as String!
            let _age = toInt32(data.subdata(in: Range(68..<72)))
            let _gender = toInt32(data.subdata(in: Range(72..<76)))
            let _emotion = toInt32(data.subdata(in: Range(76..<80)))
            
            DispatchQueue.main.async {
                self.name.text = _name
                
                var emotion_text = ""
                if(_emotion == 1){
                    emotion_text = "Happy"
                }else if(_emotion == 2){
                    emotion_text = "Netural"
                }else if(_emotion == 3){
                    emotion_text = "Upset"
                }
                self.emotion.text = emotion_text
                
                self.age.text = String(_age)
                
                if(_gender == 1){
                    self.gender.text = "Boy"
                }else if(_gender == 2){
                    self.gender.text = "Girl"
                }
            }
        }
        
    }
    
    @IBOutlet weak var avatar: UIImageView!
    
    @IBOutlet weak var name: UILabel!
    
    @IBOutlet weak var age: UILabel!
    
    @IBOutlet weak var gender: UILabel!
    
    @IBOutlet weak var emotion: UILabel!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.

        var socket: GCDAsyncUdpSocket?
        let queue = DispatchQueue.global(qos: .default)
        socket = GCDAsyncUdpSocket(delegate: self, delegateQueue: queue)
        do {
            try socket?.bind(toPort: 4243)   //Listening Port
            try socket?.beginReceiving()      //Start To Receiving Data
            
            //let data = Data(base64Encoded: "Hello World")
            //socket?.send(data??, toHost: "127.0.0.1", port: 4243, withTimeout: 10, tag: 1)
        } catch {
            print("Failed to create socket.")
        }
        
        avatar.image = UIImage(named: "Image")
        name.text = "nil"
        age.text = "24"
        gender.text = "boy"
        emotion.text = "Sleepy"
    }

    

}

