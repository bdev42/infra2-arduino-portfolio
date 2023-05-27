/**
 * MonckyCode v1.5 (20210322)
 */

const UNO_INSTRUCTIONS_PER_LINE = 4;
const UNO_LINE_INDENT = 4;
const UNO_CHARACTERS_PER_LINE = UNO_LINE_INDENT + (UNO_INSTRUCTIONS_PER_LINE * 8) + 1;

function uno_addPaddingAndEndLine(line) {
    if(line.length >= UNO_CHARACTERS_PER_LINE) {
        console.warn("Too long line given to pad, line length: ", line.length, " expected max line length: ", UNO_CHARACTERS_PER_LINE-1);
        return line+"\\\n";
    }
    return line + " ".repeat((UNO_CHARACTERS_PER_LINE-1)-line.length) + "\\\n";
}

var d=document;
function l(s){console.log(s)}

var msgArea, srcArea, binArea, hexArea, numArea, txtArea;
window.addEventListener("load", init, false);

function init(){
    //get textareas for later use and add event listeners
    msgArea=d.getElementById("messages");
    srcArea=d.getElementById("source");
    binArea=d.getElementById("bin");
    numArea=d.getElementById("linenumbers");
    addArea=d.getElementById("address");
    hexArea=d.getElementById("hex");
    txtArea=d.getElementById("plaintext");
    srcArea.addEventListener("input", translate, false);
    //fix scrolling
    srcArea.addEventListener("scroll", scroll, false);
    numArea.addEventListener("scroll", scroll, false);
    binArea.addEventListener("scroll", scroll, false);
    hexArea.addEventListener("scroll", scroll, false);
    msgArea.addEventListener("scroll", scroll, false);
    addArea.addEventListener("scroll", scroll, false);
    srcArea.focus();

    //tab interface
    var tablinks = d.getElementsByClassName("tablink");
    for (var i=0; i<tablinks.length; i++)
        tablinks[i].addEventListener("click", tabclicked, false);
    showTab(1);


}


//tab interface
function tabclicked(e){
    showTab(e.target.id.substring(4));e.preventDefault();
    srcArea.focus();
}
function showTab(number){
    var panels = d.getElementsByClassName("panel");
    for (var i=0; i<panels.length; i++)
        panels[i].style.display="none";
    d.getElementById("panel"+number).style.display = "block";
    var links = d.getElementsByClassName("tablink");
    for (var j=0; j<links.length; j++){
        var li = links[j].parentNode.parentNode;
        li.style.backgroundColor="#EEE";
    }
    var sel = d.getElementById("link"+number).parentNode.parentNode;
    sel.style.backgroundColor="#777";
}

//auto scroll other textareas when source is scrolled
function scroll(){
    numArea.scrollTop=srcArea.scrollTop;
    binArea.scrollTop=srcArea.scrollTop;
    hexArea.scrollTop=srcArea.scrollTop;
    msgArea.scrollTop=srcArea.scrollTop;
    addArea.scrollTop=srcArea.scrollTop;
}

//each keystroke the entire file is parsed
function translate(){
    binArea.value = "";
    msgArea.value = "";
    hexArea.value = "";
    numArea.value = "";
    addArea.value = "";
    //fix double newlines
    srcArea.value = srcArea.value.replace(/\n{3,}/g,"\n\n");
    //get source and decode
    var s = srcArea.value;
    var lines = s.toLowerCase().split("\n");
    var hexCodes = [], labels = [], withLabel = [], errors = [];
    txtArea.value =                 "Assembly             |#  |Hex  |Interpretation                                " + "\n"
    txtArea.value = txtArea.value + "-------------------- |-- |---- |--------------------------------------------- " + "\n"

    //all of this needs to be rewritten
    //instructions should be kept in [] and displayed after parsing and replacing labels
    for (var i=0; i<lines.length; i++){
        var line = lines[i].trim().replace(/^[0-9A-Da-d]{1,2}\s*:\s*/, "");
        //line numbers
        numArea.value = numArea.value + (i+1) + "\n";
        //store labels
        var message = "";
        if (line.isLabel()){
            var labelName = line.replace(/\s/g,"_").toUpperCase(), labelAddress = hexCodes.length;
            labelAddress = "0x" + labelAddress.toString(16).toUpperCase();
            if (labels[labelName]===undefined){
                labels[labelName] =  labelAddress;
                message = "LABEL" + labelName + " = " + labelAddress;
                lines[i]=labelName;
            }else {
                message = "ERROR: Label " + labelName + " has been used before "  + labels[labelName];
                lines[i]=labelName;
                errors.push([i,message]);
            }


        }
        //skip empty/comment/label lines
        if (line.length == 0 || line.charAt(0) == ";" || line.isLabel()) {
            binArea.value = binArea.value + "\n";
            hexArea.value = hexArea.value + "\n";
            msgArea.value = msgArea.value + message + "\n";
            addArea.value = addArea.value + "\n";
            //include these lines in plaintext
            if (line.length < 20)
                line = line.pad(20, " ", false) + ";|   |     |";
            txtArea.value = txtArea.value + line + "\n";
            continue;
        }
        //code address is zero based so get it before starting
        var address = hexCodes.length.toString(16).toUpperCase().pad(2,"0",true);
        //decode each line and output
        var instruction = new Instruction(line);
        var textline = instruction.toString().pad(20) + ";|" + address.pad(2) + " |" + instruction.toHexString()+ " |" + instruction.getMessage() + "\n";
        //if instruction has label, keep track and try again when all lines are read
        if (instruction.hasLabel()) {
            withLabel.push([i,hexCodes.length]);
            hexCodes.push("0000");
            txtArea.value = txtArea.value + textline;
            binArea.value = binArea.value +  "\n";
            hexArea.value = hexArea.value +  "\n";
            addArea.value = addArea.value + address + "\n";
        } else if (instruction.isValid()){
            hexCodes.push(instruction.toHexString());
            //update textareas
            binArea.value = binArea.value + instruction.toBinaryString() + "\n";
            hexArea.value = hexArea.value + instruction.toHexString() + "\n";
            addArea.value = addArea.value + address + "\n";
            txtArea.value = txtArea.value + textline;
        } else {
            errors.push([i, instruction.getMessage()]);
            binArea.value = binArea.value +  "\n";
            hexArea.value = hexArea.value +  "\n";
            addArea.value = addArea.value + "\n";
        }
        msgArea.value = msgArea.value + instruction.getMessage() + "\n";
    }
    //fix lines with labels
    var bin = binArea.value.split("\n");
    var hex = hexArea.value.split("\n");
    var msg = msgArea.value.split("\n");
    for (var j=0; j<withLabel.length; j++){
        var n = withLabel[j][0];
        var code = lines[n];
        var toReplace = /:\w*/.exec(code)[0];
        if (labels[toReplace.toUpperCase()] === undefined) {
            message = "ERROR: Label "+toReplace.toUpperCase() + " has not been defined";
            message.fontcolor("#D00");
            msg[n] = message;
            errors.push([n,message]);
        }else{
            instruction = new Instruction(code.replace(toReplace, labels[toReplace.toUpperCase()].toLowerCase()));
            hex[n]=instruction.toHexString();
            bin[n]=instruction.toBinaryString();
            hexCodes[withLabel[j][1]] = instruction.toHexString();
        }
    }
    binArea.value = bin.join("\n");
    hexArea.value = hex.join("\n");
    msgArea.value = msg.join("\n");
    scroll();
    for (var label in labels){
        txtArea.value = txtArea.value.replace(label, label+" ;= " + labels[label]);
    }

    var hexCopy = [...hexCodes];
    //generate LogiSim output
    var lsfArea = d.getElementById("lsfile");
    var lsfile = "v2.0 raw\n";
    while (hexCodes.length > 7) {
        var hline = hexCodes.shift();
        for (var k=0; k<7; k++){
            hline = hline + " " + hexCodes.shift();
        }
        lsfile = lsfile + hline + "\n";

    }
    lsfile = lsfile + hexCodes.join(" ") + "\n";
    lsfArea.value = lsfile;

    hexCodes = [...hexCopy];
    //generate MonckyUno program.h output
    var unoArea = d.getElementById("programh");
    var unoFile = "//program.h generated by MonckyCode 9000 v1.5\n\n";
    unoFile += uno_addPaddingAndEndLine("#define PROGRAM {");
    var inst = 0;
    var unoLine;
    while (hexCodes.length > 0) {
        if(inst === 0) unoLine = " ".repeat(UNO_LINE_INDENT);

        unoLine += "0x" + hexCodes.shift() + ", ";
        inst++;

        if(inst >= UNO_INSTRUCTIONS_PER_LINE) {
            unoFile += uno_addPaddingAndEndLine(unoLine);
            inst = 0;
        }
    }
    if(inst > 0 && inst < UNO_INSTRUCTIONS_PER_LINE) unoFile += uno_addPaddingAndEndLine(unoLine);
    unoFile += "}\n";
    unoArea.value = unoFile;

    //generate labels for info
    var lblArea = d.getElementById("labels");
    lblArea.value = "No labels";
    if (Object.keys(labels).length > 0){
        lblArea.value = "Labels ("+ Object.keys(labels).length + "): \n\n";
        for (label in labels){
            lblArea.value = lblArea.value + label + " = " + labels[label] + "\n";
        }
    }
    //generate errors
    var errArea = d.getElementById("errors");
    errArea.value = "No Errors";
    if (errors.length > 0){
        errArea.value = "Errors ("+ errors.length+ "):\n\n";
        for (k=0; k<errors.length; k++){
            errArea.value = errArea.value + String(errors[k][0]) + ": " + errors[k][1].replace("Error: ", "") + "\n";
        }
        txtArea.value = "ERRORS occured in code, see Info.";
        lsfArea.value = "ERRORS occured in code, see Info.";
        unoArea.value = "ERRORS occured in code, see Info.";
    }
}

//contructor does the actual decoding
//variables should be private, only use methods
function Instruction(code){
    this.code = code.split(";")[0]; // 1 code line w/o comments
    var words = this.code.split(/\s/);
    var operation = words[0];
    this.error = false;
    this.label = false;
    this.message = "";

    //look up instruction in codemap or generate error
    if (this.codeMap[operation] === undefined){
        this.message  = "ERROR: Unknown instruction: " + operation;
        this.error = true;
    }
    else {
        this.number = this.codeMap[operation][0];
        this.message = this.codeMap[operation][1];
    }
    this.number *= Math.pow(2, 12); // opcode is first 4 bits of 16

    //parse operands
    var operands=words.slice(1).join("").split(",");
    if (operands[0].length == 0) operands.shift();
    var op1 = String(operands[0]), op2 = String(operands[1]);
    var opneeded = 0; //check for right number of operands
    // parse operands and add to number
    // instructions with same structure are grouped together
    switch (operation){
        case "halt":
            break;
        case "li":
            opneeded = 2;
            if (!op1.isRegister()) {
                this.message = "ERROR: Invalid register: " + op1;
                this.error = true;
            }
            //if literal is label fill in later
            else if (op2.isLabel()) {
                this.label = true;
            }
            else if (!op2.is8bitNumber()) {
                this.message = "ERROR: Invalid number: " + op2;
                this.error = true;
            }
            else {
                this.number += parseInt(op1.substr(1));
                this.number += parseInt(op2) * Math.pow(2, 4);
            }
            break;
        //load/store
        case "ld":
        case "st":
            opneeded = 2;
            if (!op1.isRegister()) {
                this.message = "ERROR: Invalid register: " + op1;
                this.error = true;
            }
            else if (!op2.isAddress()) {
                this.message = "ERROR: Invalid address: " + op2;
                this.error = true;
            }
            else {
                this.number +=parseInt(op1.substr(1)) * Math.pow(2, 4);
                this.number +=parseInt(op2.match(/\d+/));

            }
            break;
        //PC operations
        case "jp":
        case "pcto":
        case "inc":
            opneeded = 1;
            if (!op1.isRegister()) {
                this.message = "ERROR: Invalid register: " + op1;
                this.error = true;
            }

            else {
                this.number += parseInt(op1.substr(1));
            }
            break;
        //ALU functions with 2 ops
        case "nop":
        case "or":
        case "and":
        case "xor":
        case "add":
        case "sub":
        case "shl":
        case "shr":
        case "ashr":
	case "not":
	case "neg":
            opneeded = 2;
            if (!op1.isRegister()) {
                this.message = "ERROR: Invalid register: " + op1;
                this.error = true;
            }
            else if (!op2.isRegister()) {
            this.message = "ERROR: Invalid register: " + op2;
            this.error = true;
            }
            else {

                this.number += this.codeMap[operation][2] * Math.pow(2,8);
                this.number +=parseInt(op1.substr(1)) * Math.pow(2, 4);
                this.number +=parseInt(op2.substr(1));

            }

            break;
        //conditional jumps
        //WTF Chrome//  case /jpn?[c,z,o,s]./:
        case "jpc":
        case "jpnc":
        case "jpz":
        case "jpnz":
        case "jps":
        case "jpns":
        case "jpo":
        case "jpno":
            opneeded = 1;
            if (!op1.isRegister()) {
                this.message = "ERROR: Invalid register: " + op1;
                this.error = true;
            }
            else {
                this.number += this.codeMap[operation][2] * Math.pow(2,4);
                this.number += parseInt(op1.substr(1));
            }
            break;
    }
    if (!this.error && operands.length != opneeded) {
        this.message = "ERROR: Wrong amount of parameters";
        this.error = true;
    }
    if (!this.error) this.message = this.codeMap[operation][1].replace(/%s/g,op1).replace(/%p/g,op2);
    if (this.label) this.message = this.message.replace("getal", "lijnnr");

}

//methods to retrieve different representations of instruction
Instruction.prototype.toString = function() {
    return this.code;
};
Instruction.prototype.toBinaryString = function(){
    var string = this.number.toString(2);
    while (string.length < 16) string = "0" + string;
    string = string.substr(0,4) + " " + string.substr(4,4) + " " + string.substr(8,4) + " " + string.substr(12,4);
    return string;
};
Instruction.prototype.toHexString = function(){
    var string = this.number.toString(16);
    while (string.length < 4) string = "0" + string;
    return string;
};
Instruction.prototype.isValid = function(){
    return !this.error;
};
Instruction.prototype.getMessage = function(){
    return this.message;
};
Instruction.prototype.hasLabel = function(){
    return this.label;
};
//associative array for decoding
//codemap[assembler]=[first 4 bits of machine code, message string (%s = op1, %p = op2)]
Instruction.prototype.codeMap = [];
Instruction.prototype.codeMap["halt"]=  [0x0, "Halt: Processor stops"];
Instruction.prototype.codeMap["li"]=    [0x1, "Immediate: Load number %p in reg %s"];
Instruction.prototype.codeMap["ld"]=    [0x8, "RAM: Load address %p and place in reg %s"];
Instruction.prototype.codeMap["st"]=    [0xA, "RAM: Store register %s in RAM address %p"];
Instruction.prototype.codeMap["jp"]=    [0xC, "PC: Jump to code line in %s"];
Instruction.prototype.codeMap["pcto"]=  [0xE, "PC: Store PC value in reg %s"];
Instruction.prototype.codeMap["inc"]=  [0x2, "REG: Increment %s"];
//ALU
//third element = ALU instruction code (4b)
Instruction.prototype.codeMap["nop"]=   [0x4, "ALU: No operation: %s = %p", 0x0];
Instruction.prototype.codeMap["or"]=    [0x4, "ALU: %s = %s OR %p", 0x1];
Instruction.prototype.codeMap["and"]=   [0x4, "ALU: %s = %s AND %p", 0x2];
Instruction.prototype.codeMap["xor"]=   [0x4, "ALU: %s = %s XOR %p", 0x3];
Instruction.prototype.codeMap["add"]=   [0x4, "ALU: %s = %s + %p", 0x4];
Instruction.prototype.codeMap["sub"]=   [0x4, "ALU: %s = %s - %p", 0x5];
Instruction.prototype.codeMap["shl"]=   [0x4, "ALU: %s = shift left %s , %p bits", 0x6];
Instruction.prototype.codeMap["shr"]=   [0x4, "ALU: %s = shift right %s , %p bits", 0x7];
Instruction.prototype.codeMap["ashr"]=  [0x4, "ALU: %s = arithm. shift right %s , %p bits", 0x8];
Instruction.prototype.codeMap["not"]=   [0x4, "ALU: %s = NOT %p", 0x9];
Instruction.prototype.codeMap["neg"]=   [0x4, "ALU: %s = 2-complement %p", 0xA];
//conditional jumps
//third element = condition instruction code (3b)
Instruction.prototype.codeMap["jpc"]=   [0xF, "CJMP: Jump to %s if carry flag = 1",0];
Instruction.prototype.codeMap["jpnc"]=  [0xF, "CJMP: Jump to %s if no carry flag",1];
Instruction.prototype.codeMap["jpz"]=   [0xF, "CJMP: Jump to %s if result ALU = 0 ",2];
Instruction.prototype.codeMap["jpnz"]=  [0xF, "CJMP: Jump to %s if result ALU != 0",3];
Instruction.prototype.codeMap["jps"]=   [0xF, "CJMP: Jump to %s if result ALU < 0",4];
Instruction.prototype.codeMap["jpns"]=  [0xF, "CJMP: Jump to %s if result ALU >= 0",5];
Instruction.prototype.codeMap["jpo"]=   [0xF, "CJMP: Jump to %s if overflow",6];
Instruction.prototype.codeMap["jpno"]=  [0xF, "CJMP: Jump to %s if no overflow",7];

//extend String to easily check for correct numbers, registers, addresses
String.prototype.is8bitNumber = function(){
    return /^\d{1,4}$|^(0x[0-9,a-f]{1,2})$/.test(this) && parseInt(this)<256 && parseInt(this)>=0;
};
String.prototype.isRegister = function(){
    return /^r\d{1,2}$/.test(this) && parseInt(this.substr(1))<16 && parseInt(this.substr(1))>=0;
};
String.prototype.isAddress = function(){
    return /^\(\w*\)$/.test(this) && this.substring(1,this.length-1).isRegister();
};
String.prototype.isLabel = function(){
    return /^:\w*$/.test(this)
};
//extend String to make fixed length strings
String.prototype.pad = function(length,fill,left){
    if (!fill) fill = " ";
    if (this.length == length) return this;
    var result = (left)? fill+ this : this +fill;
    while (result.length < length) result = (left)? fill+ result : result +fill;
    return result;
};


