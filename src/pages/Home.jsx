'use strict';
import logo from '../assets/logo.png';
import Table from '../components/Table'

const T = () => Table('Hello Stranger', [{key: 'aaaaaaaaaaaaaaaaa', value: 'baaaaaaaaaaaa'}, {key: 'c', value: 'd'}])


export default function() {
  return (
    <div>
      <div class={'flex justify-center flex-auto'}>
        <T/>
      </div>
      <img src={logo} alt='logo' />
    </div>
  );
};

