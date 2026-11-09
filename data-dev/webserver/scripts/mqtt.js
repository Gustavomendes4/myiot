

export function main(){

    console.log("MQTT script loaded");

}

function submit_Callback(){

    /* Get forms data and convert to JSON */
    let form = document.getElementById("mqtt-form");
    let formData = new FormData(form);
    let jsonData = {};

    for (const [key, value] of formData.entries()) {
        jsonData[key] = value;
    }


    /* print */
    console.log("Form data:", jsonData);





    

}

/* Test button callback */
function testCallback(){
    // let form = document.getElementById("mqtt-form");
    // let formData = new FormData(form);
    // let jsonData = {};

    console.log("Test button clicked");
}

