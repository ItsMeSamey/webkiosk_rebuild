'use strict';
import SideBar from './SideBar';
import {SideBarComponent} from './SideBar';

// temprary convenience function 
const _ = (n,p) => ({name:n, path:p});

// Default components list
const file = <svg width="24" height="24" viewBox="0 0 24 24"><path d="M19.903 8.586a.997.997 0 0 0-.196-.293l-6-6a.997.997 0 0 0-.293-.196c-.03-.014-.062-.022-.094-.033a.991.991 0 0 0-.259-.051C13.04 2.011 13.021 2 13 2H6c-1.103 0-2 .897-2 2v16c0 1.103.897 2 2 2h12c1.103 0 2-.897 2-2V9c0-.021-.011-.04-.013-.062a.952.952 0 0 0-.051-.259c-.01-.032-.019-.063-.033-.093zM16.586 8H14V5.414L16.586 8zM6 20V4h6v5a1 1 0 0 0 1 1h5l.002 10H6z"/><path d="M8 12h8v2H8zm0 4h8v2H8zm0-8h2v2H8z"/></svg>;
const book = <svg width="24" height="24" viewBox="0 0 24 24"><path d="M2 7v1l11 4 9-4V7L11 4z"/><path d="M4 11v4.267c0 1.621 4.001 3.893 9 3.734 4-.126 6.586-1.972 7-3.467.024-.089.037-.178.037-.268V11L13 14l-5-1.667v3.213l-1-.364V12l-3-1z"/></svg>;
const credit_card = <svg width="24" height="24" viewBox="0 0 24 24"><path d="M20 4H4c-1.103 0-2 .897-2 2v12c0 1.103.897 2 2 2h16c1.103 0 2-.897 2-2V6c0-1.103-.897-2-2-2zM4 6h16v2H4V6zm0 12v-6h16.001l.001 6H4z"/><path d="M6 14h6v2H6z"/></svg>;
const archive = <svg width="24" height="24" viewBox="0 0 24 24"><path d="m21.706 5.292-2.999-2.999A.996.996 0 0 0 18 2H6a.996.996 0 0 0-.707.293L2.294 5.292A.994.994 0 0 0 2 6v13c0 1.103.897 2 2 2h16c1.103 0 2-.897 2-2V6a.994.994 0 0 0-.294-.708zM6.414 4h11.172l1 1H5.414l1-1zM4 19V7h16l.002 12H4z"/><path d="M14 9h-4v3H7l5 5 5-5h-3z"/></svg>;
const log_out = <svg width="24" height="24" viewBox="0 0 24 24"><path d="M19.002 3h-14c-1.103 0-2 .897-2 2v4h2V5h14v14h-14v-4h-2v4c0 1.103.897 2 2 2h14c1.103 0 2-.897 2-2V5c0-1.103-.898-2-2-2z"/><path d="m11 16 5-4-5-4v3.001H3v2h8z"/></svg>;

const sb_array = [
  SideBarComponent('Exam', file, [_('Marks', 'marks'), _('Grades', 'grades'), _('Timetable', 'timetable')]),
  SideBarComponent('Academics', book, [_('Document Status', 'document-status'), _('Subject Info', 'subject-info'),_('Attendance', 'attendance'), _('Hostel Allotment', 'hostel-allotment'),]),
  SideBarComponent('Finance', credit_card, [_('Fees', 'fees'), _('SRS', 'srs')]),
  SideBarComponent('Archive', archive, 'archive'),
  SideBarComponent('Logout', log_out, 'log-out'),
];

// cant call SideBar(sb_array) directly because it is requires root element to be mounted
export default () => SideBar(sb_array)

