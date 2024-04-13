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
  tables = dom.getElementsByTagName('tbody');
  const kvp = Array.from(tables[0].getElementsByTagName(td))
    .map(table_row => Array.from(table_row.getElementsByTagName('td')).map(_ => _.innerText.trim())
  );
  return {
    name: 'unknown',
    enrollment_no: 'unknown',
    dob: 'unknown',
    course: 'unknown',
    semester: 'unknown',
    cell_no: 'unknown',
    phone_no: 'unknown',
    email: 'unknown',
    lms_username: 'unknown',
    lms_initial_password: 'unknown',
    father_name: 'unknown',
    mother_name: 'unknown',
    parent_cell_no: 'unknown',
    parent_phone_no: 'unknown',
    parent_email: 'unknown',
    current_address: 'unknown',
    current_district: 'unknown',
    current_city_and_pin: 'unknown',
    current_state: 'unknown',
    permanant_address: 'unknown',
    permanant_district: 'unknown',
    permanant_city_and_pin: 'unknown',
    permanant_state: 'unknown',
    mentor_name: 'unknown',
    mentor_department: 'unknown',
    mentor_room: 'unknown',
    mentor_cell: 'unknown',
    mentor_email:'unknown'
  }
}
