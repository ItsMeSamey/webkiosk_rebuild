'use strict';
import logo from '../assets/logo.png';
import styles from '../assets/App.module.css';
import Table from '../components/Table'

const T = () => Table('Hello Stranger', [{key: 'aaaaaaaaaaaaaaaaa', value: 'baaaaaaaaaaaa'}, {key: 'c', value: 'd'}])

function Home() {
  return (
    <div class={styles.App}>
      <div class={"flex justify-center flex-auto"}>
        <T/>
      </div>
      <img src={logo} class={styles.logo} alt="logo" />
    </div>
  );
}

export default Home;

