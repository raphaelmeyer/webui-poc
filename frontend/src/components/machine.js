import React, { Component } from 'react'

class Machine extends Component {
  constructor(props) {
    super(props);
    this.state = {
      state: null,
      temperature: null
    };
    this.timer = null
  }

  componentDidMount() {
    this.fetch_data();
    this.timer = setInterval(() => this.fetch_data(), 1000)
  }

  componentWillUnmount() {
    clearInterval(this.timer)
  }

  fetch_data() {
    fetch('http://localhost:8011/machine/state', { method: 'GET' })
      .then(res => res.json())
      .then((ms) => {

        fetch('http://localhost:8011/machine/temperature', { method: 'GET' })
          .then(res => res.json())
          .then((mt) => {
            this.setState({
              state: ms.state,
              temperature: Number.parseFloat(mt.temperature).toFixed(2)
            })
          })
      })
      .catch(console.log)


  }

  handle_action(action) {
    fetch('http://localhost:8011/machine/' + action, { method: 'POST' })
      .then(this.fetch_data())
      .catch(console.log)
  }

  machine_state() {
    return 'machine-' + this.state.state
  }

  render_button() {
    switch (this.state.state) {
      case 'off':
      case 'idle':
        return (
          <button onClick={() => this.handle_action('start')}>start</button>
        )
      case 'running':
        return (
          <button onClick={() => this.handle_action('stop')}>stop</button>
        )
      default:
        return (
          <button disabled>N/A</button>
        )
    }
  }

  render() {
    return (
      <React.Fragment>
        <div className="content-box">
          <h1>Machine</h1>
        </div>
        <div className="content-box">
          <table>
            <tr>
              <td>State:</td>
              <td className={this.machine_state()}>{this.state.state}</td>
            </tr>
            <tr>
              <td>Temperature:</td>
              <td>{this.state.temperature}</td>
            </tr>
          </table>
        </div>
        <div className="content-box">
          {this.render_button()}
        </div>
      </React.Fragment>
    )
  }

}

export default Machine
