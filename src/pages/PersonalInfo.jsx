'use strict';
import { For } from "solid-js";
import Table from '../components/Table';

const _ = (k, v, e) => ({key: k, value: v, editable:e });

function PersonalInfo({name, enrollment_no, dob, course, semester}){
  return Table('Personal Information', [
    _('Name', name),
    _('Date of Birth', dob),
    _('Enrollment No.', enrollment_no),
    _('Course', course),
    _('Semester', semester)
  ]);
}

// Email is only Personal email (i.e. not thapar email)
function StudentInfo({cell_no, phone_no, email ,lms_username, lms_initial_password}){
  return Table('Student Contact detail', [
    _('LMS Username', lms_username),
    _('LMS Initial Password', lms_initial_password),
    _('Cell No.', cell_no, true),
    _('Phone No.', phone_no, true),
    _('Email', email, true),
  ]);
}

function ParentInfo({father_name, mother_name, parent_cell_no, parent_phone_no, parent_email}){
  return Table('Parent Contact detail', [
    _('Father Name', father_name),
    _('Mother Name', mother_name),
    _('Cell No.', parent_cell_no),
    _('Phone No.', parent_phone_no),
    _('Email', parent_email),
  ]);
}

function CurrentAddressInfo({current_address, current_district, current_city_and_pin, current_state}){
  return Table('Correspondence Address', [
    _('Address', current_address),
    _('District', current_district),
    _('City/Pin', current_city_and_pin),
    _('State', current_state),
  ]);
}

function PermanantAddressInfo({permanant_address, permanant_district, permanant_city_and_pin, permanant_state}){
  return Table('Correspondence Address', [
    _('Address', permanant_address),
    _('District', permanant_district),
    _('City/Pin', permanant_city_and_pin),
    _('State', permanant_state),
  ]);
}

function MentorInfo({mentor_name, mentor_department, mentor_room, mentor_cell, mentor_email}){
  return Table('Mentor Information', [
    _('Name', mentor_name),
    _('Department', mentor_department),
    _('Room', mentor_room),
    _('Cell No./Email', mentor_cell + mentor_email),
  ]);
}


export default function(){
  console.log('NOOOOOOOOOOOO');
  let json = {
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
  const arr = [PersonalInfo(json), StudentInfo(json), ParentInfo(json), CurrentAddressInfo(json), PermanantAddressInfo(json), MentorInfo(json)]
      //   style={{
      //   'grid-template-columns': 'repeat(auto-fit, minmax(200px, 1fr))',
      //   'grid-auto-rows': 'minmax(100px, auto)',
      // }}

  return(
    <div
      class="ml-4 mt-4 grid gap-4 lg:grid-cols-3 max-lg:grid-cols-2 max-[540px]:grid-cols-1">
      <For each={arr}>
        {_=>_}
      </For>
    </div>
  );
}


