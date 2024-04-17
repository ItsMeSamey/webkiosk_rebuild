'use strict';

import SideBar from './SideBar';
import { SideBarComponent } from './SideBar';

// cant call SideBar(sb_array) directly because it is requires root element to be mounted


export default () => {
  // IMPORTANT; otherwise causes recomputation each time DefaultBar is loaded;
  if (globalThis.__.DefaultBar) return globalThis.__.DefaultBar;
  // temprary convenience function 
  const _ = (n,p) => ({name:n, path:p});

  // Default components list
  const file = <svg width="24" height="24" viewBox="0 0 24 24"><path d="M18 22a2 2 0 0 0 2-2V8l-6-6H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12zM13 4l5 5h-5V4zM7 8h3v2H7V8zm0 4h10v2H7v-2zm0 4h10v2H7v-2z"/></svg>;
  const book = <svg width="24" height="24" viewBox="0 0 24 24"><path d="M2 7v1l11 4 9-4V7L11 4z"/><path d="M4 11v4.267c0 1.621 4.001 3.893 9 3.734 4-.126 6.586-1.972 7-3.467.024-.089.037-.178.037-.268V11L13 14l-5-1.667v3.213l-1-.364V12l-3-1z"/></svg>;
  const credit_card = <svg width="24" height="24" viewBox="0 0 24 24"><path d="M20 4H4c-1.103 0-2 .897-2 2v2h20V6c0-1.103-.897-2-2-2zM2 18c0 1.103.897 2 2 2h16c1.103 0 2-.897 2-2v-6H2v6zm3-3h6v2H5v-2z"/></svg>;
  const archive = <svg width="24" height="24" viewBox="0 0 24 24"><path d="m21.706 5.292-2.999-2.999A.996.996 0 0 0 18 2H6a.997.997 0 0 0-.707.293L2.294 5.292A.996.996 0 0 0 2 6v13c0 1.103.897 2 2 2h16c1.103 0 2-.897 2-2V6a.994.994 0 0 0-.294-.708zM6.414 4h11.172l1 1H5.414l1-1zM12 18l-5-5h3v-3h4v3h3l-5 5z"/></svg>;
  const account = <svg width="24" height="24" viewBox="0 0 24 24"><path d="M20 2H8a2 2 0 0 0-2 2v12a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V4a2 2 0 0 0-2-2zm-6 2.5a2.5 2.5 0 1 1 0 5 2.5 2.5 0 0 1 0-5zM19 15H9v-.25C9 12.901 11.254 11 14 11s5 1.901 5 3.75V15z"/><path d="M4 8H2v12c0 1.103.897 2 2 2h12v-2H4V8z"/></svg>;

  const sb_array = [
    SideBarComponent('Account', account, [_('Personal Info', 'personal-info'), _('Change Password', 'change-password'), _('Logout', 'log-out')]),
    SideBarComponent('Exam', file, [_('Marks', 'marks'), _('Grades', 'grades'), _('Timetable', 'timetable')]),
    SideBarComponent('Academics', book, [_('Document Status', 'document-status'), _('Subject Info', 'subject-info'),_('Attendance', 'attendance'), _('Hostel Allotment', 'hostel-allotment'),]),
    SideBarComponent('Finance', credit_card, [_('Fees', 'fees'), _('SRS', 'srs')]),
    SideBarComponent('Archive', archive, 'archive'),
  ];
  
  globalThis.__.DefaultBar = SideBar(sb_array);
  return globalThis.__.DefaultBar;
};

