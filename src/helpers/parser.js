'use strict';


async function login(roll_number, password, is_parent){
  const response = await fetch("/auth/" +roll_number+"/"+password+"/"+(is_parent ? "P": "S"));
  console.log(response);
  if (response.ok) {
    const decoder = new TextDecoder();
    return decoder.decode((await (await (response.body.getReader())).read()).value);
  }
}

async function call(cookie, path){
  const response = await fetch("/api/" + cookie + "/" + path);
  console.log(response);
  if (response.ok) {
    const decoder = new TextDecoder();
    const parser = new DOMParser();
    const dom = decoder.decode((await (await (response.body.getReader())).read()).value);
    return parser.parseFromString(dom, "text/html");
  }
}

globalThis.__get = (url = 'StudentFiles/PersonalFiles/StudPersonalInfo.jsp') => {
  login('102303535', '')
  .then(cookie => {
    call(cookie, url).then(console.log)
  });
}

function ParsePersonalInfo(dom){
  const tables = dom.getElementsByTagName('tbody');
  const kvp = Array.from(tables[0].getElementsByTagName('tr'))
    .map(table_row =>Array.from(table_row.getElementsByTagName('td')).map(_ => _.innerText.trim())
  );
  console.log(kvp);
  return {
    name: kvp[0][1],
    enrollment_no: kvp[1][1],
    dob: kvp[4][1],
    course: kvp[5][1],
    semester: kvp[6][1],
    cell_no: kvp[8][1],
    phone_no: kvp[9][1],
    email: kvp[10][1],
    lms_username: kvp[12][1],
    lms_initial_password: kvp[13][1],
    father_name: kvp[2][1],
    mother_name: kvp[3][1],
    parent_cell_no: kvp[8][3],
    parent_phone_no: kvp[9][3],
    parent_email: kvp[10][3],
    current_address: kvp[19][1],
    current_district: kvp[20][1],
    current_city_and_pin: kvp[21][1],
    current_state: kvp[22][1],
    permanant_address: kvp[19][3],
    permanant_district: kvp[20][3],
    permanant_city_and_pin: kvp[21][3],
    permanant_state: kvp[22][3],
    mentor_name: kvp[15][1],
    mentor_department: kvp[16][1],
    mentor_room: kvp[17][1],
    mentor_cell: kvp[15][3],
    mentor_email:kvp[16][3]
  }
}

